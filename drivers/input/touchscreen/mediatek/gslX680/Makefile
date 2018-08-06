# Linux driver folder
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/gslX680/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/

ifeq ($(CONFIG_64BIT),y)
obj-y += gsl_point_id_arm64.o
$(obj)/gsl_point_id_arm64.o: $(srctree)/$(obj)/gsl_point_id_arm64
	cp $(srctree)/$(obj)/gsl_point_id_arm64  $(obj)/gsl_point_id_arm64.o
else
obj-y += gsl_point_id.o
$(obj)/gsl_point_id.o: $(srctree)/$(obj)/gsl_point_id
	cp $(srctree)/$(obj)/gsl_point_id  $(obj)/gsl_point_id.o
endif

obj-y	+=  mtk_gslX680.o


