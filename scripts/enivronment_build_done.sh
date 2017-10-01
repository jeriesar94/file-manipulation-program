#!/bin/bash
script_path=$(readlink -f -- "$0")
script_directory=${script_path%/*}
#<------General predefined paths for builds----->
BUSY_BOX="busybox-1.26.2"
IOZONE="iozone3_465"
LINUX_KERNEL="linux-4.11"
QEMU="qemu-2.9.0"
OUT="out"
#<------------END OF PATHS---------------------->
#START CODE
echo
echo "Starting to build linux on QEMU environment..."
echo "Please specify build TOP Directory: (Example: PATH/\$TOP_DIRECTORY)"
#Reading the path for TOP Directory
read -e DIR
if [[ ! -d $DIR ]]; then
	echo "Directory does not exist, creating directory..."
	mkdir -p "$DIR"
fi
cd $DIR
echo -e "\nDownloading Needed archives..."
if [[ ! -e "busybox-1.26.2.tar.bz2" ]]; then
echo "Downloading BusyBox..."

curl -# -f --connect-timeout 80 -m 600 -o busybox-1.26.2.tar.bz2 https://www.busybox.net/downloads/busybox-1.26.2.tar.bz2
if [ $? != 0 ]; then
	echo "ERROR: cannot download BusyBox."
	echo "Exiting..."
	exit -1
fi
fi
echo "-Downloaded BusyBox-"
echo 
echo
if [[ ! -e "iozone3_465.tar" ]]; then
echo "Downloading IOZONE..."

curl -# -f --connect-timeout 80 -m 600 -o iozone3_465.tar http://www.iozone.org/src/current/iozone3_465.tar
if [ $? != 0 ]; then
	echo "ERROR: cannot download IOZONE."
	echo "Exiting..."
	exit -1
fi
fi
echo "-Downloaded IOZONE-"
echo
echo
if [[ ! -e "linux-4.11.tar.xz" ]]; then
echo "Downloading Linux Kernel v4.11 ..."

curl -# -f --connect-timeout 80 -m 600 -o linux-4.11.tar.xz https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.11.tar.xz
if [ $? != 0 ]; then
	echo "ERROR: cannot download Linux Kernel."
	echo "Exiting..."
	exit -1
fi
fi
echo "-Downloaded Linux Kernel-"
echo
echo
if [[ ! -e "qemu-2.9.0.tar.xz" ]]; then
echo "Downloading QEMU v2.9.0 ..."

curl -# -f --connect-timeout 80 -m 600 -o qemu-2.9.0.tar.xz https://download.qemu.org/qemu-2.9.0.tar.xz
if [ $? != 0 ]; then
	echo "ERROR: cannot download QEMU."
	echo "Exiting..."
	exit -1
fi
fi
echo "-Downloaded QEMU-"
echo
echo
echo "Packages downloaded successfully."
echo "Extracting archives..."
echo 
echo "Extracting 'busybox-1.26.2.tar.bz2'"
tar xf busybox-1.26.2.tar.bz2
if [ $? != 0 ]; then 
	echo "ERROR: Problem with extracting archive."
	echo "Exiting..."
	exit -2
fi
echo "Success."
echo 
echo "Extracting 'iozone3_465.tar'"
tar xf iozone3_465.tar
if [ $? != 0 ]; then 
	echo "ERROR: Problem with extracting archive."
	echo "Exiting..."
	exit -2
fi
echo "Success."
echo 
echo "Extracting 'linux-4.11.tar.xz'"
tar xf linux-4.11.tar.xz
if [ $? != 0 ]; then 
	echo "ERROR: Problem with extracting archive."
	echo "Exiting..."
	exit -2
fi
echo "Success."
echo 
echo "Extracting 'qemu-2.9.0.tar.xz'"
tar xf qemu-2.9.0.tar.xz
if [ $? != 0 ]; then 
	echo "ERROR: Problem with extracting archive."
	echo "Exiting..."
	exit -2
fi
echo "Success."
echo
echo "Archives Extracted Successfully."
echo
#=================BusyBox======================
echo "Building BusyBox..."
cd $BUSY_BOX
mkdir -p ../out/busybox-x86
#Before make menuconfig , check if curses.h exists
#Query the dpkg database for the libncurses5-dev package
dpkg-query -s libncurses5-dev >/dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "curses.h is not found! packages libncurses5-dev and libncursesw5-dev are needed."
	echo "installing packages..."
	sudo apt-get -qq install libncurses5-dev libncursesw5-dev > /dev/null
	echo "packages installed."
fi
echo
echo "A configuration menu will now open,"
echo
echo -e "configure options - choose a static binary build and exit while saving the
'.config' file (the rest of the options can be left with their default settings)\n
    		-> Busybox Settings\n
        		-> Build Options\n
           			[*] Build BusyBox as a static binary (no shared libs)"
echo
make O=../out/busybox-x86 menuconfig -s 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make menuconfig'."
	echo "Stopping..."
	exit -3
fi
echo "Building BusyBox binary..."
make O=../out/busybox-x86 -s > /dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make', cannot build BusyBox."
	echo "Stopping..."
	exit -3
fi
make O=../out/busybox-x86 install -s > /dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make install', cannot build BusyBox."
	echo "Stopping..."
	exit -3
fi
echo "Done."
echo
cd ../$OUT
echo "Creating Directory strucutre, default scripts and set-up files..."
mkdir -p initramfs-x86
cd initramfs-x86
mkdir -p {bin,sbin,etc,proc,sys,usr/{bin,sbin},tmp,mnt/{1,2,3,4},testing}
echo "Copying BusyBox binary and links..."
cp -a ../busybox-x86/_install/* .
echo "Creating the passwords file..."
echo "root::0:0:root:/root:/bin/sh" > ../initramfs-x86/etc/passwd
echo
echo -e "#!/bin/sh\n
mount -t proc none /proc\n
mount -t sysfs none /sys\n
mount none /sys/kernel/debug/ -t debugfs\n
mount none /tmp -t tmpfs\n
# populate '/dev/ directory\n
mdev -s\n
# start shell in an endless loop\n
echo -e \"\n\n** miniboot started **\n\"\n
while true\n
	do echo \"Starting a new shell.. (up $(cut -d' ' -f1 /proc/uptime) seconds)\"\n
        # note - 'ENV=/.shinit' is optional and used for defining various aliases\n
        # using the user defined '.shinit' script\n
        ENV=/.shinit /bin/sh -l\n
done" > init
chmod +x ./init
echo -e "# a shortcut to quit the emulation environment cleanly\n
alias q='poweroff -f'\n

alias ll='ls -alF'\n
alias lt='ls -altrF'\n

# more user defined aliases can be easily defined here..\n" > .shinit
#=================IOZONE======================
echo "Building IOZONE..."
cd ../../$IOZONE/src/current
chmod a+w makefile

cat "$script_directory"/makefile_edited > makefile
chmod a-w makefile
#Query the dpkg database for the libssl-dev package
dpkg-query -s libssl-dev >/dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "opensslv.h is not found! package libssl-dev is needed."
	echo "installing package..."
	sudo apt-get -qq install libssl-dev > /dev/null
	echo "package installed."
fi
make linux-AMD64 -s > /dev/null 2> /dev/null 
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make linux-AMD64', cannot build IOZONE."
	echo "Stopping..."
	exit -3
fi
cp -a ./iozone ../../../out/initramfs-x86/testing/
cd ../../../$OUT
echo "find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs-busybox-x86.cpio.gz" > generate_initramfs.sh
cd ./initramfs-x86
. ../generate_initramfs.sh #> /dev/null 2> /dev/null
echo -e "Done.\n"
#=================Kernel======================
echo "Preparing for Linux Kernel building..." 
cd ../../$LINUX_KERNEL
mkdir -p ../out/kernel
make O=../out/kernel mrproper -s > /dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make mrporper', cannot build Kernel."
	echo "Stopping..."
	exit -3
fi
make O=../obj/linux-x86-basic x86_64_defconfig -s > /dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make x86_64_defconfig', cannot build Kernel."
	echo "Stopping..."
	exit -3
fi
echo "Configuring Kernel..."
echo
echo "Another menu will now open, "
echo -e "# config options - change the following settings and save '.config'\n
#\n
#     -> General setup\n
#        -> Local version - append to kernel release\n
#            set to 'example1'\n
#\n
#    -> Device drivers\n
#        <*> MMC/SD/SDIO card support\n
#            <*>   MMC block device driver\n
#            <*>   Secure Digital Host Controller Interface support\n
#            <*>   SDHCI support on PCI bus\n
#\n
#    -> File systems\n
#        -*- Miscellaneous filesystems\n
#            <*>   F2FS filesystem support (EXPERIMENTAL)\n
#\n
#    -> Kernel hacking\n
#        [*] Compile the kernel with frame pointers\n
#        [*] Compile the kernel with debug info\n
#        [*] KGDB: kernel debugger\n
#            <*>   KGDB: use kgdb over the serial console\n
#\n"
make O=../out/kernel menuconfig -s 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make menuconfig', cannot build Kernel."
	echo "Stopping..."
	exit -3
fi
echo 
echo "Building Kernel..."
echo
echo "info: Kernel building process can be optimized by using multiple cores, how many cores do you want to use?"
echo "WARNING: It is not recommended to choose a number more than physical cores."
read cores
echo
echo "Building Started..."
echo
make -j$cores O=../out/kernel -s > /dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make', cannot build Kernel."
	echo "Stopping..."
	exit -3
fi
echo "Done Building the kernel."
echo
#=================QEMU======================
echo "Building QEMU..."
cd ../$QEMU
mkdir ../out/qemu-build
cd ../out/qemu-build
#Query the dpkg database for the libglib2.0-dev and zlib1g-dev packages
dpkg-query -s libglib2.0-dev >/dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "glib-2.22 is not found! package libglib2.0-dev is needed."
	echo "installing package..."
	sudo apt-get -qq install libglib2.0-dev > /dev/null
	echo "package installed."
fi
dpkg-query -s zlib1g-dev >/dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "gthread-2.0 is not found! package zlib1g-dev is needed."
	echo "installing package..."
	sudo apt-get -qq install zlib1g-dev > /dev/null
	echo "package installed."
fi
echo -e "\nConfiguring QEMU to build a x86_64 target..."
../../$QEMU/configure --target-list=x86_64-softmmu --prefix="$PWD/../qemu-out" > /dev/null
#Query the dpkg database for the autoconf and dh-autoreconf packages
dpkg-query -s autoconf >/dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "autoconf is not found! package autoconf is needed."
	echo "installing package..."
	sudo apt-get -qq install autoconf > /dev/null
	echo "package installed."
fi
dpkg-query -s dh-autoreconf >/dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "autoreconf is not found! package dh-autoreconf is needed."
	echo "installing package..."
	sudo apt-get -qq install dh-autoreconf > /dev/null
	echo "package installed."
fi
echo -e "\nBuilding started...\n"
echo "info: QEMU building process can be optimized by using multiple cores, how many cores do you want to use?"
echo "WARNING: It is not recommended to choose a number more than physical cores."
read cores
make -j$cores > /dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make', cannot build QEMU."
	echo "Stopping..."
	exit -3
fi
#Query the dpkg database for the flex and bison packages
dpkg-query -s flex >/dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "flex is not found! package flex is needed."
	echo "installing package..."
	sudo apt-get -qq install flex > /dev/null
	echo "package installed."
fi
dpkg-query -s bison >/dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "bison is not found! package bison is needed."
	echo "installing package..."
	sudo apt-get -qq install bison > /dev/null
	echo "package installed."
fi
make -j$cores install -s > /dev/null 2> /dev/null
if [ $? != 0 ]; then
	echo
	echo "Error in command 'make install', cannot build QEMU."
	echo "Stopping..."
	exit -3
fi
echo -e "\nDone building QEMU."
echo -e "\nLinux on QEMU system has been built.\nEnjoy!"
#wget -q -O "$DIR"/busybox-1.26.2.tar.bz2 https://www.busybox.net/downloads/buswybox-1.26.2.tar.bz2
