import os
import shutil
import subprocess
import tkinter as tk
from tkinter import filedialog, messagebox

def find_vulkan_sdk():
    vulkan_sdk = os.environ.get('VULKAN_SDK')
    if not vulkan_sdk:
        print("VULKAN_SDK environment variable is not set.")
        return None
    return vulkan_sdk

def compile_shader(shader_path, vulkan_sdk):
    glslc_path = shutil.which('glslc', path=os.path.join(vulkan_sdk, 'Bin'))
    if not os.path.isfile(glslc_path):
        print(f"glslc not found at {glslc_path}.")
        return False
    
    output_path = shader_path + ".spv"
    result = subprocess.run([glslc_path, shader_path, '-o', output_path], capture_output=True, text=True)
    
    if result.returncode == 0:
        print(f"Successfully compiled {shader_path} to {output_path}.")
        return True
    else:
        print(f"Failed to compile {shader_path}:\n{result.stderr}")
        return False

def main():
    vulkan_sdk = find_vulkan_sdk()
    if not vulkan_sdk:
        return

    root = tk.Tk()
    root.withdraw()
    
    file_paths = filedialog.askopenfilenames(title="Select Shader Source Files", filetypes=[("GLSL files", "*.vert *.frag *.comp *.geom *.tesc *.tese"), ("All files", "*.*")])
    
    if not file_paths:
        messagebox.showinfo("Info", "No files selected.")
        return

    for file_path in file_paths:
        compile_shader(file_path, vulkan_sdk)

if __name__ == "__main__":
    main()
