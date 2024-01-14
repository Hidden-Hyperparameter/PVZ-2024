from PIL import Image
from PIL.GifImagePlugin import ImageSequence
import os

def extract_gif_frames(gif_path, output_folder):
    # 确保输出文件夹存在
    if not os.path.exists(output_folder):
        return

    # 打开GIF文件
    images = Image.open(gif_path)
    i = 0
    for frame in ImageSequence.Iterator(images):
        # 保存每一帧为 PNG 文件
        frame.save(os.path.join(output_folder, f'{i}.png'), 'PNG')
        i += 1

# 使用函数
relative_path = 'c:\\Users\\ZHH2\\Downloads\\images\\images\\Zombies\\Zombie\\'
extract_gif_frames(relative_path+'ZombieAttack.gif', relative_path)
