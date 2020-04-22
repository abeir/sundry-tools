#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import argparse
import os
import time
from multiprocessing import cpu_count
from string import Template

HOME_PATH = os.environ['HOME']
BIN_PATH = HOME_PATH + '/.bin'

TARGET_SHORT_NAME = 'os4_1_1'


def commands():
    parser = argparse.ArgumentParser(allow_abbrev=False, description='编译syberh项目')

    subparsers_cmd = parser.add_subparsers(dest='cmd', help='commands')
    # clear
    parser_clear = subparsers_cmd.add_parser('clear')
    parser_clear.add_argument('-b', '--build', required=True,
                        dest='build_path', action='store',
                        help='编译输出路径')
    # build
    parser_build = subparsers_cmd.add_parser('build')
    parser_build.add_argument('-p', '--pro', required=True,
                        dest='pro_path', action='store',
                        help='项目主pro文件路径')
    parser_build.add_argument('-b', '--build', required=True,
                        dest='build_path', action='store',
                        help='编译输出路径')
    parser_build.add_argument('-n', '--num', required=False,
                        dest='process_num', action='store', type=int, 
                        help='并行编译的进程数')
    parser_build.add_argument('-d', '--pdk', required=True,
                        dest='pdk_path', action='store',
                        help='pdk根目录')
    parser_build.add_argument('--args', required=False,
                        dest='args', action='append',
                        help='qmake额外参数')              
    # install
    parser_install = subparsers_cmd.add_parser('install')
    parser_install.add_argument('-s', '--sop', required=True,
                        dest='sop_path', action='store',
                        help='sop包路径')
    # rename
    parser_rename = subparsers_cmd.add_parser('rename')
    parser_rename.add_argument('-s', '--sop', required=True,
                        dest='sop_path', action='store',
                        help='改名的sop包路径')
    return parser


def run_commands(cmds = {}):
    parser = commands()
    args = parser.parse_args()

    if not args.cmd:
        print(parser.print_help())
        raise SystemExit(1)

    for sub_cmd, cmd_func in cmds.items():
        if sub_cmd == args.cmd:
            cmd_func(args)


def search_sop(dir_path):
    if not os.path.exists(dir_path):
        return None
    for f in os.listdir(dir_path):
        print(f)


def need_package_sop(pro_path):
    pro_dir = os.path.dirname(pro_path)
    xml_path = os.path.join(pro_dir, 'sopconfig.xml')
    return os.path.exists(xml_path) and os.path.isfile(xml_path)
    

def build(args):
    if not os.path.exists(args.build_path):
        os.makedirs(args.build_path)
    if not os.path.exists(args.pro_path) and not os.path.isfile(args.pro_path):
        print('未找到pro文件：' + args.pro_path)
        raise SystemExit(1)
    if not os.path.exists(args.pdk_path):
        print('未安装pdk，不存在的位置：' + args.pdk_path)
        raise SystemExit(1)

    os.chdir(args.build_path)

    kchroot_path = os.path.join(args.pdk_path, 'sdk/script/kchroot')
    target_full_name = 'target-armv7tnhl-' + TARGET_SHORT_NAME
    qmake_path = '/usr/lib/qt5/bin/qmake'
    process_num = args.process_num
    if not process_num:
        process_num = cpu_count()

    qmake_args = ''
    if args.args and len(args.args) > 0:
        qmake_args = ' '.join(args.args)
    qmake_args = qmake_args.rstrip()

    cmd_setup1 = Template("${kchroot_path} 'sb2 -t ${target_full_name} -R' '${qmake_path} ${pro_path} -r -spec linux-g++ CONFIG+=qml_debug ${qmake_args}'") \
                .substitute(kchroot_path=kchroot_path, target_full_name=target_full_name, qmake_path=qmake_path, pro_path=args.pro_path, qmake_args=qmake_args)

    cmd_setup2 = Template("${kchroot_path} 'sb2 -t ${target_full_name} -R' '/usr/bin/make -j${process_num}'") \
                .substitute(kchroot_path=kchroot_path, target_full_name=target_full_name, process_num=process_num)

    cmd_setup3 = Template("${kchroot_path} 'sb2 -t ${target_full_name} -R' 'buildpkg ${pro_path}'") \
                .substitute(kchroot_path=kchroot_path, target_full_name=target_full_name, pro_path=args.pro_path)

    print(cmd_setup1)

    os.system(cmd_setup1)
    os.system(cmd_setup2)
    if need_package_sop(args.pro_path):
        os.system(cmd_setup3)


def clear(args):
    if os.path.exists(args.build_path):
        print('开始清理目录：' + args.build_path)
        time.sleep(2)
        os.system('rm -rf ' + args.build_path + '/*')
    else:
        print('清理的目录不存在：' + args.build_path)


def install(args):
    if not os.path.exists(args.sop_path) or not os.path.isfile(args.sop_path):
        print('sop不存在：', args.sop_path)
        raise SystemExit(1)
    if not args.sop_path.endswith('.sop'):
        print('不是sop包：', args.sop_path)
        raise SystemExit(1)
    os.system('expect ' + os.path.join(BIN_PATH, 'syberh-scp') + ' ' + args.sop_path)


def rename(args):
    if not os.path.exists(args.sop_path) or not os.path.isfile(args.sop_path):
        print('sop不存在：', args.sop_path)
        raise SystemExit(1)

    sop_dir = os.path.dirname(args.sop_path)
    sop_filename = os.path.basename(args.sop_path)
    new_sop_filename = sop_filename.rstrip('.sop') + '.tar.gz'
    os.rename(args.sop_path, os.path.join(sop_dir, new_sop_filename))


if __name__ == '__main__':
    run_commands({
        'build': build,
        'clear': clear,
        'install': install,
        'rename': rename
    })
