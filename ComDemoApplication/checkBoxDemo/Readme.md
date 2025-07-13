
# checkBoxDemo

tinyPiXOS 提供的复选框演示应用。

---

## 安装流程

```bash
mkdir build
cd build
cmake ../
make 
make install
```

## 说明

1. 安装过程中即make install过程中，按照如下步骤执行文件安装拷贝
  - 首先会根据平台不同向 tinyPiXApp/install/install_x86_64 或 tinyPiXApp/install/install_arm64文件夹拷贝应用的安装文件，例如应用图标、运行文件、配置文件等等；用于对应用进行打包
  - 将所有安装文件（文件内容与第一条相同）安装至系统目录
2. ***.json 为应用运行配置json。可手动配置相关参数，或者由打包工具生成。
