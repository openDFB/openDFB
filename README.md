# OpenDFB
## Background
DirectFB官方已经不再维护，但目前还有不少用户使用DirectFB，该组件从某种程度上可以说是业界事实标准，之所以这么流行原因是DirectFB天生就是针对嵌入式设备而设计，以最小的资源来实现了最高的硬件加速性。如今IOT设备流行，需要一种在RAM和ROM占用均小图形显示组件，所以该项目从DirectFB-1.6.3拉出了一个分支，命名OpenDFB，该项目主要是通过对DirectFB代码进行重构和精简，旨在建立一个IOT轻量级设备的显示服务组件。

## Ideas
OpenDFB定位为IOT设备显示服务组件，因为DirectFB发展多年，一方面存在历史代码导致代码陈旧，另外一方面也存在发展中增加了很多背离最早轻量级设计目标的很多没有用的功能，OpenDFB出现就是为了解决这些问题，主要是通过明确显示服务的目标删除一些没有用的功能，甚至对外接口也会有些调整，目前调整的代码包括：
1. 将directfb内部C++代码统一整改成纯C代码，避免代码膨胀，保持代码简洁；(done)
2. 删除历史遗留的半吊子组件，包括++dfb, dvc, fusiondale, fusionsound, voodoo, input hub;  (done)
3. 显示服务需要强化显示功能，对于显示之外的功能进行删除，包括IDirectFBVideoProvider,IFusionSound,IFusionSoundMusicProvider,IWater；（done）
4. 对外接口保留，但内部实现保留最简单实现，建议应用端实现， 包括Image provider, Font/Text. （done）
5. 对于2D接口保留Blitter相关功能，对于其他矢量绘制等功能均删除. （done）
6. 将游离directfb之外的库收编到内部直接编译，包括sawman, divine;（done）
7. 将内核fusion机制使用socket来代替，避免依赖linux内核依赖，方便移植到其他RTOS。（doing）

## Key specifications
OpenDFB关注以下功能，其他功能和规格均会被删除或不会被纳入
1. 提供显示输出功能，包括图形和视频输出；
2. 提供交互输入设备功能，包括按键，触摸，鼠标等输入设备对接；
3. 提供窗口管理功能；
4. 提供2D Blitter图形加速功能，其他绘制接口不会提供；
5. 支持单进程和多进程架构；

## How to use OpenDFB

### Build Requirements

   Mandatory are
     - libc
     - libpthread
     - libm
     - libdl

   For regenerating autofoo (./autogen.sh or autoreconf)
     - autoconf
     - automake
     - libtool
     - pkg-config

   Optionally, depending on the configuration you want:

   FBDev
     - Linux kernel 2.2.x or newer with working frame buffer device
       (check /proc/fb) for the fbdev system.

   SDL
     - libSDL (Simple Direct Media Layer) for the sdl system.

   X11
     - libX11 (X11 client library) for the X11 system (libx11-dev and libxext-dev packages).

   The following libraries are optional, but important (Debian package names):

   Fonts
     - libfreetype6-dev for TrueType and other fonts

   Images
     - libjpeg-dev for Joint Picture Expert Group images
     - libpng-dev for Portable Network Graphics

   Extra
     - zlib1g-dev for compressed screenshot support (also needed by libpng)

   The multi application core also requires <linux/fusion.h>, see the
   section "Running multiple DirectFB applications" for more details.

   The build of the image and font providers can be disabled but we
   strongly suggest that you don't do this since the code examples and
   a lot of DirectFB applications depend on the functionality provided
   by them.

### Usage Requirements

   Depending on the DirectFB application you want to run, you need some
   or all of these:

   - A working frame buffer device (check the output of 'fbset -i').
   - A keyboard (if it works on the console, everything should be fine).
   - A PS/2 or serial mouse for windowing. USB and ADB mice do also work
     via PS/2 emulation.

   Using the single application core you always need access to /dev/tty0,
   /dev/fb0 and the mouse device (/dev/psaux, /dev/mouse).
   You can either run all DirectFB applications as root or allow users
   to access these devices. A reasonable way to do this is to add users
   to the group tty (or some other group) and allow this group to read
   and write the files in /dev:

       crw-rw----    1 root     tty       29,   0  /dev/fb0
       crw-rw----    1 root     tty       10,   1  /dev/psaux
       crw-r-----    1 root     tty        4,   0  /dev/tty0

   If you are using the multi application core, only the master process needs
   access to all of these devices. Additional processes (slaves) just need
   access to /dev/fb0 and:

       crw-rw----    1 root     fusion    29,   0  /dev/fusion/0
   
   Note that the master creates a shared memory file, probably
   in '/dev/shm/fusion.0'. It's read/writeable for anyone matching
   the master process' user or group.


   Some applications from the DirectFB-examples package have additional
   requirements:

   - A video card supported by video4linux for df_video.
   - A joystick for df_joystick.


### Running multiple DirectFB applications at the same time

   With the default build of the DirectFB library, only one DirectFB
   application may be running. However you have the option to use the
   multi application core of DirectFB which allows multiple DirectFB
   applications to run at the same time within a session.

   DirectFB applications communicate through a layer we call "Fusion".
   This layer was previously implemented completely in user space using
   semaphores and message queues. But since 0.9.15 the Fusion Kernel
   Device is required which implements critical parts of Fusion and thus
   lead to better performance and decent stability. To install this kernel
   module (only available for Linux yet), find the linux-fusion
   module in our CVS or GIT repository or on the DirectFB website.
   For DirectFB 1.4.x releases you should use linux-fusion 8.1.1 or newer.

   Compile DirectFB with multi-application core enabled:

     ./configure --enable-multi

   Make sure your Linux kernel supports tmpfs. This is explained in
   the kernel sources in Documentation/filesystems/tmpfs.txt.
   Mount a tmpfs filesystem as /dev/shm:

     mount tmpfs /dev/shm -ttmpfs

   Optionally a mount point can be specified via the "tmpfs" option,
   see directfbrc(5). The option has to be the same for all processes.

   A good way to test the multi-application core is to install the lite
   toolkit and DFBTerm, a DirectFB terminal (both available in the
   DirectFB CVS/GIT repository). You can then start DirectFB applications
   from dfbterm.

### Installation

   1) In the DirectFB directory type:

	./configure <options>
	make
	make install (as superuser)

      Use './configure --help' to get a list of possible configure options.

      Imported options include:
        --enable-multi   Enables the Multi Application Core
        --enable-debug   Enables many debug messages and assertions
        --enable-trace   Enables run time stack trace information

      Debugging and especially stack trace support are a performance
      impact. It may be noticably slower in some areas, e.g. text drawing.

      You may use the options "no-debug" and "no-trace" by default, e.g.
      in '<prefix>/etc/directfbrc', and use "--dfb:debug,trace" on the command
      line if needed.

   2) Make sure that "<prefix>/lib" is listed in your /etc/ld.so.conf.
      The default prefix is "/usr/local". After adding it you have to
      run 'ldconfig' as superuser.

      Alternatively, you can add the path to the environment
      variable LD_LIBRARY_PATH. This is useful for temporarily
      switching to another installed version.

   3) You might want to copy fb.modes to /etc or merge it with your
      existing /etc/fb.modes file. The first entry will be used by
      default - copy other entries you may need.

   4) If you want to use a serial mouse, create a link called /dev/mouse
      that points to serial device (e.g. /dev/ttyS0). Then add a line
      describing your mouse-protocol to /etc/directfbrc or ~/.directfbrc:
      "mouse-protocol=[ms|ms3|mouseman|mousesystems]".

   5) If you are cross compiling and have installed the required libraries
      someplace other than /usr/local/lib be sure to add the bin directory
      for those libraries to the front of your 'PATH'.

        For instance if you configured using:
          --prefix=/dfb/usr/local
        then be sure to
          export PATH=/dfb/usr/local/bin:$PATH
        before compiling and installing.


### Configuring DirectFB

   There are lots of things that can be configured. We try to ship DirectFB
   with reasonable defaults but you might have to tweak things. There are
   several ways to do this. You may edit the system-wide configuration
   file <prefix>/etc/directfbrc or the user-specific $HOME/.directfbrc.
   There's a manual page called directfbrc(5) that documents all the settings.
   The same manual page also explains how DirectFB application can be
   configured via the command-line.


### Configuring the Linux frame buffer device

   DirectFB's prefered way-of-working needs a Linux kernel
   with frame buffer support. Check the documentation in the kernel tree
   (/usr/src/linux/Documentation/fb/) on how to enable the frame buffer device
   for your graphics card. 

   The generic VESA frame buffer device does not support mode switching
   and you will not get hardware acceleration. To make DirectFB work with
   veasfb, you should add the following lines to /etc/lilo.conf:

	append="video=vesa:ywrap,mtrr"

   'ywrap' enables panning with wraparound.
   'mtrr' enables setting caching type for the frame buffer to write-combining.

	vga=791

   This sets the mode on startup. 791 means 1024x768@16, 788 means 800x600@16.

   All VESA Video Modes:

   Bits    640x480 800x600 1024x768 1280x1024 1600x1200
   8       769     771     773      775       796
   16      785     788     791      794       798
   32      786     789     792      795       799

   Other frame buffer devices support mode switching. DirectFB will only 
   support modes listed in your /etc/fb.modes file. By default the first 
   entry found is used.

   If you have a Matrox card you may want to try the vsync patch found in
   the patches directory that enables applications to "idle wait" for the
   vertical retrace.


### Using the builtin "window manager"

   Since DirectFB lacks a real window manager, we added a hack to the
   window stack to allow for basic window management. While pressing
   the <Meta> (or Windows) key or alternatively <Caps-Lock> you can do
   the following:

     - Drag your mouse to move the focused window.
     - Drag and press <Ctrl> to resize the focused window.
     - Drag and press <Alt> to change the opacity of the focused window.
     - Press C to close the focused window.
     - Press A to lower the focused window to the bottom.
     - Press X to cycle the focus through the windows.
     - Press S to raise the lowest window to the top.
     - Press P to enable and show the mouse cursor.
     - Press R to rotate the focused window by 90 degree.
     - Press E to focus the window currently under the mouse cursor,
       useful in conjunction with 'X'.
     - Press Escape to return from fullscreen mode to the desktop.
       (currently not advisable if the fullscreen app is still flipping)
     - Press F12 to redraw the whole window stack.

   You might want to use the "capslock-meta" option (see directfbrc(5))
   if you don't have a <Meta> key.


### Documentation

   A complete API reference documentation in HTML format is created during 
   the build in the docs directory. You may also access the API reference
   as well as a concepts overview, tutorials and the FAQ online at
   https://github.com/openDFB/OpenDFB/wiki. 

