# SM9_FREE

标识密码算法SM9（IBC）实现，包括密钥生成、签名验签、密钥交换和加解密等SM9标准中提到的所有功能。

该实现基于[Miracl密码库](https://github.com/miracl/MIRACL)

采用C语言编写，可支持X86、X86_64、ARM等多平台编译。

# 源码关系
 [Miracl密码库](https://github.com/miracl/MIRACL)比较庞大，因此只截取出其中最核心的部分，包括大整数运算，Fp，Fp^2，Fp^4域计算以及Fp和Fp^2上椭圆曲线基础预算等功能。

依赖的所有[Miracl密码库](https://github.com/miracl/MIRACL)文件在文件夹 [SM9_FREE/miracl](https://github.com/songgeng87/SM9_FREE/tree/master/SM9_FREE/miracl) 内。

在 [Miracl密码库](https://github.com/miracl/MIRACL) 基础之上，实现了满足SM9扩域需求的Fp^12和ate-pairing实现，最后在此之上完成了SM9的密钥生成、签名验签、密钥交换和加解密等功能。
所有和SM9相关的文件都在文件夹 [SM9_FREE/sm9](https://github.com/songgeng87/SM9_FREE/tree/master/SM9_FREE/sm9)内。

[SM9Test.c](https://github.com/songgeng87/SM9_FREE/tree/master/SM9_FREE/SM9Test.c)文件内是简单的测试用例，同时对性能做了简单的统计。

# SM9编译测试
可以直接用XCODE进行编译测试

也可用gcc进行编译

make test

./test

# SM9使用方法
TODO

# 开源协议
本代码遵循BSD开源协议，欢迎大家使用

# 性能
在cpu i7 2.3G, 64位单线程环境下：

    签名10000次大约耗时：120秒
    
    验证10000次大约耗时：372秒
    
    加密10000次大约耗时：130秒
    
    解密10000次大约耗时：214秒
    
    密钥交换20000次大约耗时：833秒

# 捐献
接受ETH捐献：
ETH地址：[0x36D112ad944c44Be528D5B799ECC131ffD3669BA](https://etherscan.io/address/0x36D112ad944c44Be528D5B799ECC131ffD3669BA)

接受EOS捐献：
EOS账号：[jueduijuxing](https://eosflare.io/account/jueduijuxing)

所有收到的捐献将以徐峥、刘青云、周迅或张韶涵的名义在西部助建希望小学、水井等基础设施（不知以他人名义捐献是否妥当，如有不妥将删除）。
