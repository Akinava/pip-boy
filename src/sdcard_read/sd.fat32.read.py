#!/usr/bin/env python3
import sys
import subprocess
import struct


DIR=b'BIN        '
FILE=b'BOOT    BIN'
byte_per_sector = 0x0200
vol_map_address = 0x1c6
vol_map_address_len = 4
sector_befor_fat_tab_addr = 0x0e
sector_befor_fat_tab_len = 2
fat_tables_count_addr = 0x10
fat_tables_count_len = 1
fat_tables_sectors_size_addr = 0x24
fat_tables_sectors_size_len = 2

fat_obj_data_len = 32

dev='/dev/sdb'
byte_per_cluster = None
secrors_per_cluster_addr = 0x0d
sectors_per_cluster_len = 1

obj_fat_name_addr = 0
obj_fat_name_len = 8+3

obj_fat_cluster_addr = 0x1a
obj_fat_cluster_len = 2
obj_fat_size_addr = 0x1c
obj_fat_size_len = 4


def raw_sd_read(addr_from, size=1):
    return subprocess.check_output(
        ['dd',
         'if={}'.format(dev),
         'bs=1',
         'skip={}'.format(addr_from),
         'count={}'.format(size)], stderr=subprocess.DEVNULL)


def unpuck_volum(vol):
    byte_order = '<'
    unpack_rule = {1: 'B', 2: 'H', 4: 'I', 8:'Q'}
    return struct.unpack(byte_order + unpack_rule[len(vol)], vol)[0]


def get_volume_addres(vol_num):
    if vol_num >= 4: return None
    sectors = unpuck_volum(raw_sd_read(
        vol_map_address+vol_num*0x0f,
        vol_map_address_len))
    if sectors == 0: return None
    return sectors * byte_per_sector


def get_addr_fat32_data(addr_vol):
    define_byte_per_cluster(addr_vol)

    sectors_before_firts_fat_table = unpuck_volum(
        raw_sd_read(addr_vol+sector_befor_fat_tab_addr,
                    sector_befor_fat_tab_len))
    fat_tables_count = unpuck_volum(
        raw_sd_read(addr_vol+fat_tables_count_addr,
                    fat_tables_count_len))
    fat_tables_sectors_size = unpuck_volum(
        raw_sd_read(addr_vol+fat_tables_sectors_size_addr,
                    fat_tables_sectors_size_len))
    return addr_vol + byte_per_sector * (
        sectors_before_firts_fat_table +
        fat_tables_sectors_size * fat_tables_count)


def define_byte_per_cluster(addr_vol):
    spc = unpuck_volum(
        raw_sd_read(addr_vol+secrors_per_cluster_addr,
                    sectors_per_cluster_len))
    global byte_per_cluster
    byte_per_cluster = spc * byte_per_sector


def get_addr_fat_obj(target_name, addr, addr_fat32_data):
    name = ''
    while name != target_name:
        obj_data = raw_sd_read(addr, fat_obj_data_len)

        if obj_data == b'\x00' * fat_obj_data_len:
            print('obj not exist')
            exit(1)

        name = obj_data[:obj_fat_name_len]
        addr += fat_obj_data_len

    cluster_addr = unpuck_volum(obj_data[obj_fat_cluster_addr:obj_fat_cluster_addr+obj_fat_cluster_len])
    obj_size = unpuck_volum(obj_data[obj_fat_size_addr:obj_fat_size_addr+obj_fat_size_len])
    print ('cluster addr for', target_name, hex(cluster_addr), obj_size)
    return (cluster_addr-2)*byte_per_cluster+addr_fat32_data, obj_size


def read_file(addr, size):
    return raw_sd_read(addr, size)


if __name__ == "__main__":
    addr_vol = get_volume_addres(0)
    if addr_vol is None:
        print('volume not found')
        exit(1)
    print('vol addr', hex(addr_vol))
    addr_fat32_data = get_addr_fat32_data(addr_vol)
    print('fat data addr', hex(addr_fat32_data), 'byte_per_cluster', hex(byte_per_cluster))
    addr_dir, _ = get_addr_fat_obj(DIR, addr_fat32_data, addr_fat32_data)
    print ('obj addr', hex(addr_dir))
    addr_file, size_file = get_addr_fat_obj(FILE, addr_dir, addr_fat32_data)
    print ('obj addr', hex(addr_file), 'size', hex(size_file))
    print ('file:', read_file(addr_file, size_file))
