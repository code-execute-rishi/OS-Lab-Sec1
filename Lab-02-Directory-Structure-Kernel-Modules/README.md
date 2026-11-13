# Experiment 2: UNIX/Linux Directory Structure, Kernel Modules & Hardware Diagnostics

**Aim:**
1. Part A: To explore and comprehend the UNIX/Linux Directory Structure defined by the Filesystem Hierarchy Standard (FHS), focusing on /bin, /dev, /etc, /home, /lib, /tmp, and /usr, and visualize directory nesting via the tree utility.
2. Part B: To investigate the internal Linux kernel tree (/lib/modules, kernel drivers, header trees, documentation), define Loadable Kernel Modules (LKMs) and .ko files, and analyze diagnostic commands: modprobe -l, lsmod, cat /proc/cpuinfo, cat /proc/meminfo, cat /proc/partitions, and lspci.

Commands and outputs below are copied from the lab file (Ubuntu 22.04 VM). The theory and explanation tables are in the lab file PDF.

### 2.2 Visualizing Directory Hierarchy with the tree Command

```
aradhya@aradhya-os-vm:~$ tree -d -L 2 /
/
├── bin -> usr/bin
├── boot
│   ├── efi
│   └── grub
├── dev
│   ├── block
│   ├── char
│   ├── cpu
│   ├── disk
│   ├── input
│   ├── net
│   └── pts
├── etc
│   ├── default
│   ├── modprobe.d
│   ├── network
│   ├── systemd
│   └── udev
├── home
│   └── aradhya
├── lib -> usr/lib
├── lib64 -> usr/lib64
├── proc
├── root
├── run
├── sys
├── tmp
└── usr
    ├── bin
    ├── include
    ├── lib
    ├── local
    ├── sbin
    ├── share
    └── src
```

```
aradhya@aradhya-os-vm:~$ ls -F /lib/modules/$(uname -r)/kernel
arch/ crypto/ drivers/ fs/ kernel/ lib/ net/ sound/
aradhya@aradhya-os-vm:~$ ls -F /lib/modules/$(uname -r)/kernel/drivers | head -n 8
ata/ base/ block/ char/ cpufreq/ crypto/ dma/ gpu/
```

### Command 1: modprobe -l

```
aradhya@aradhya-os-vm:~$ head -n 4 /lib/modules/$(uname -r)/modules.dep
kernel/drivers/net/ethernet/intel/e1000/e1000.ko:
kernel/drivers/block/loop.ko:
kernel/fs/ext4/ext4.ko: kernel/fs/jbd2/jbd2.ko kernel/fs/mbcache.ko kernel/lib/crc16.ko
kernel/drivers/virtio/virtio.ko:
```

### Command 2: lsmod

```
aradhya@aradhya-os-vm:~$ lsmod | head -n 7
Module                  Size Used by
vboxguest             393216 4
e1000                 151552 0
ext4                 1003520 1
jbd2                  163840 1 ext4
crc16                  16384 1 ext4
psmouse               176128 0
```

### 5.1 Processor Diagnostics: cat /proc/cpuinfo

```
aradhya@aradhya-os-vm:~$ cat /proc/cpuinfo | grep -E "model name|cpu MHz|cache size|cpu cores" | head -n 4
model name      : Apple Silicon / VirtualBox Virtual CPU @ 2.60GHz
cpu MHz         : 2592.000
cache size      : 4096 KB
cpu cores       : 2
```

### 5.2 Physical & Virtual Memory: cat /proc/meminfo

```
aradhya@aradhya-os-vm:~$ cat /proc/meminfo | head -n 5
MemTotal:        4039216 kB
MemFree:         2512480 kB
MemAvailable:    2948128 kB
Buffers:           84512 kB
Cached:           629456 kB
```

### 5.3 Storage Block Partitions: cat /proc/partitions

```
aradhya@aradhya-os-vm:~$ cat /proc/partitions
major minor #blocks name

   8         0   26214400 sda
   8         1     524288 sda1
   8         2   25689088 sda2
```

### 5.4 PCI Peripheral Interconnect Bus Scan: lspci

```
aradhya@aradhya-os-vm:~$ lspci
00:00.0 Host bridge: Intel Corporation 440FX - 82441FX PMC [Natoma]
00:01.0 ISA bridge: Intel Corporation 82371SB PIIX3 ISA [Natoma/Triton II]
00:02.0 VGA compatible controller: VMware SVGA II Adapter
00:03.0 Ethernet controller: Intel Corporation 82540EM Gigabit Ethernet Controller
00:04.0 System peripheral: InnoTek Systemberatung GmbH VirtualBox Guest Service
00:07.0 Bridge: Intel Corporation 82371AB/EB/MB PIIX4 ACPI
00:0d.0 SATA controller: Intel Corporation 82801HM/HEM (ICH8M) SATA Controller [AHCI mode]
```
