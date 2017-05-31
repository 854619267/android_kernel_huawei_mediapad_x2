# android_kernel_huawei_mediapad_x2
  export CROSS_COMPILE=arm-linux-androideabi-
	export PATH=$PATH:<toolchain_parent_dir>/arm-linux-androideabi-4.6/bin
  make ARCH=arm hisi_k3v3oem1_defconfig
  make ARCH=arm zImage
