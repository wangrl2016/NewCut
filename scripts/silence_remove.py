import argparse
import os
import shutil
import subprocess

"""
删除声音文件中的无声片段

传递输入、输出文件夹（需要有读、写权限）
e.g. python silence_remove.py -i input_dir -o output_dir
"""

# 时长超过2s，且连续时间内的阈值低于43dB的片段算无声片段
silence_duration = 2.0
silence_threshold_db = -43.0

# 保留截取片段前后0.5s
silence_margin = 0.5


def audio_duration(audio_file_path):
    p = subprocess.run(['ffprobe', '-v', 'error', '-show_entries', 'format=duration',
                        '-of', 'default=noprint_wrappers=1:nokey=1', '-i', audio_file_path],
                       check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)

    duration = float(p.stdout.strip('\n'))
    return duration


def audio_intercept(file_path, start, end, index=0):
    """
    从片段中截取对应时间戳的片段
    :param file_path: 输入文件
    :param start: 起始点
    :param end: 结束点
    :param index: 片段标号
    :return: 新的文件路径，不满足要求的返回空
    """
    if end <= start or end - start <= silence_margin:
        return ''

    temp_file_path = os.path.splitext(file_path)[0] + '_000_0_' + str(index) + os.path.splitext(file_path)[1]
    subprocess.run(['ffmpeg', '-i', file_path, '-ss', str(start), '-to', str(end),
                    '-c', 'copy', temp_file_path],
                   stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return temp_file_path


def audio_concat(intercept_audio_files, new_file_path):
    """
    将所有文件拼接起来
    :param intercept_audio_files: 需要拼接的文件
    :param new_file_path: 生成的文件
    :return: 生成文件的路径
    """

    # # 只能使用文件进行连接
    # concat_str = 'concat:'
    # for file in intercept_audio_files:
    #     concat_str += file
    #     concat_str += '|'
    # # 去掉最后的竖线
    # concat_str = concat_str[:-1]
    #
    # subprocess.run(['ffmpeg', '-i', concat_str, '-c', 'copy', new_file_path],
    #                stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    list_file_dir = new_file_path[0: new_file_path.rfind('/') + 1]
    # 递归创建目录
    if not os.path.exists(list_file_dir):
        os.makedirs(list_file_dir)
    list_file = os.path.join(list_file_dir, 'list_file.txt')
    f = open(list_file, 'w')
    for file in intercept_audio_files:
        f.write('file \'' + file + '\'\n')
    f.close()

    subprocess.run(['ffmpeg', '-f', 'concat', '-safe', '0', '-i',
                    list_file, '-c', 'copy', new_file_path],
                   stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    # 删除相关文件
    os.remove(list_file)
    for file in intercept_audio_files:
        os.remove(file)

    return new_file_path


def silence_remove(file_path, input_dir, output_dir):
    """
    移除音频文件中的无声片段（长度超过silence_duration, 响度小于silence_threshold_db），
    并保留无声片段首尾各silence_margin长度。
    :param file_path: 传入的音频文件
    :param input_dir: 输入文件夹的路径（文件夹存在嵌套）
    :param output_dir: 输出文件夹的路径（输出文件路径 = 输出文件夹的路径 + 输入文件名）
    :return: 新生成的文件路径和有声文件时长
    """
    audio_filter = 'silencedetect=n=' + str(silence_threshold_db) + 'dB:d=' + str(silence_duration)
    p = subprocess.run(['ffmpeg', '-i', file_path, '-af', audio_filter, '-f', 'null', '/dev/null'],
                       check=True, stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, universal_newlines=True)

    silence_starts = []
    silence_ends = []
    for line in p.stdout.split('\n'):
        line = line.strip('\n')
        if line.__contains__('silence_start:'):
            words = line.split(' ')
            for index, word in enumerate(words):
                if word.__contains__('silence_start:'):
                    silence_starts.append(words[index + 1])
        elif line.__contains__('silence_end:'):
            words = line.split(' ')
            for index, word in enumerate(words):
                if word.__contains__('silence_end:'):
                    silence_ends.append(words[index + 1])

    # 对时间进行校验
    if len(silence_starts) != len(silence_ends):
        print('Filter silence detect error')
        return

    # 计算有声片段的起始和结束时间
    # 有声片段的开始 = 无声片段的结束 - silence_margin
    # 有声片段的结束 = 无声片段的起始 + silence_margin
    audio_starts = [0]
    for timestamp in silence_ends:
        audio_starts.append(float(timestamp) - silence_margin)

    audio_ends = []
    for timestamp in silence_starts:
        audio_ends.append(float(timestamp) + silence_margin)
    audio_ends.append(audio_duration(file_path))

    duration = 0.0
    for index, start in enumerate(audio_starts):
        if audio_ends[index] > start:
            duration += audio_ends[index] - start

    # 截取有声片段
    intercept_audio_files = []
    for index, start in enumerate(audio_starts):
        temp = audio_intercept(file_path, start, audio_ends[index], index)
        if temp:
            intercept_audio_files.append(temp)

    # 合并有声片段，获取有声片段的路径
    if input_dir[-1] != '/':
        input_dir = input_dir + '/'
    new_file_path = os.path.join(output_dir, file_path.replace(input_dir, ''))
    audio_concat(intercept_audio_files, new_file_path)

    return [new_file_path, duration]


def main(args):
    input_dir = args.input
    output_dir = args.output

    if not os.path.isabs(input_dir):
        input_dir = os.path.join(os.getcwd(), input_dir)
    if not os.path.join(output_dir):
        output_dir = os.path.join(os.getcwd(), output_dir)

    # 创建输出目录，如果存在就删除
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.makedirs(output_dir)

    if not os.path.isdir(input_dir) or not os.path.isdir(output_dir):
        print('Input/output is not directory')
        return

    if not os.access(input_dir, os.R_OK) or not os.access(output_dir, os.W_OK):
        print('No permission for input/output directory')
        return

    audio_file_paths = []
    # 遍历所有声音文件
    if os.path.isdir(input_dir):
        for root, dirs, filenames in os.walk(input_dir):
            for file in filenames:
                ext = os.path.splitext(file)[-1]
                if ext == '.wav' or ext == '.mp3' or ext == '.m4a':
                    audio_file_paths.append(os.path.join(root, file))

    # 打印所有的声音文件
    # print(audio_file_paths)

    # 将所有有声片段时间进行相加得到总时长
    total_duration = 0.0
    for audio_file_path in audio_file_paths:
        [temp_path, duration] = silence_remove(audio_file_path, input_dir, output_dir)
        print(temp_path)
        total_duration += duration

    total_minutes = int(total_duration / 60)
    # 计算片段总时长
    print('Total duration: ' + str(int(total_minutes / 60)) + 'h' + str(int(total_minutes % 60)) + 'min')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='PROG', conflict_handler='resolve')
    parser.add_argument('-i', '--input', help='Input directory')
    parser.add_argument('-o', '--output', help='Output directory')

    main(parser.parse_args())
