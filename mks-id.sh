#! /bin/bash

if [ -d "/dev/serial/by-id" ];then
	path=$(ls /dev/serial/by-id/*)
	# echo $path
	if [ -f "/home/mks/klipper_config/MKS_THR.cfg" ];then
		# SERIALLINE=`cat /home/mks/klipper_config/MKS_THR.cfg | grep -E 'serial'`
		# serial = ${SERIALLINE#*:}
		# echo ${serial}
		sed -i "s|serial:.*|serial:"${path}"|g" /home/mks/klipper_config/MKS_THR.cfg
	fi
fi

# 删除多余的printer-* 冗余文件
CONFIG_DIR="/home/mks/klipper_config"

# 检查目录是否存在
if [ -d "$CONFIG_DIR" ]; then
    # 进入目录
    cd "$CONFIG_DIR"
    
    # 找出所有带日期的printer-*.cfg文件，按文件名中的日期排序，然后删除排在前三之外的所有文件
    ls printer-*.cfg | sort -t '-' -k 2,2r | tail -n +4 | xargs -r rm --
    ls printer_*.cfg | sort -t '_' -k 2,2r | tail -n +3 | xargs -r rm --  
fi

