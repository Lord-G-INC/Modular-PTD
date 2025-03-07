import os
import glob
import json

def genInfo(folder_path: str):
    if not os.path.isdir(folder_path):
        print(f"\033[91mError: \"{folder_path}\" is not a directory.\033[0m")
        return
    
    infoPath: str = os.path.join(folder_path, "GitFolderInfo.json")

    if os.path.isfile(infoPath):
        print(f"\033[90m{folder_path} already has a folder info file.\033[0m")
        return

    files: list = []

    for file in glob.glob(folder_path + "/**", recursive=True):
        if not os.path.isfile(file):
            continue
        
        path = os.path.relpath(file, folder_path).replace("\\", "/")

        if path == "ModuleInfo.json" or path == "GitFolderInfo.json":
            continue

        files.append(path)

    json_content: dict = {
        "Version": 1,
        "Files": files
    }

    with open(os.path.join(folder_path, "GitFolderInfo.json"), "w") as file:
        file.write(json.dumps(json_content, indent=2))

    print(f"\033[92mGenerated folder info for {folder_path}, {len(files)} files.\033[0m")

if __name__ == "__main__":
    os.chdir(os.path.dirname(__file__)) # I love python fr

    for dir in glob.glob("PTD\\*", recursive=False):
        if os.path.isdir(dir) and not os.path.exists(os.path.join(dir, ".git")):
            genInfo(dir)