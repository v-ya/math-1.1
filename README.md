# math-1.6.1

### 想说的话 v1.0 @2018.11.13
本来想做一个纯粹的数学计算工具(如名称所示)，但没想到现在越偏越远，反而连原本 1.0 时(重写之前)的微分计算积分计算都还没实现……

最后这个东西大概会变成杂七杂八不伦不类的东西吧。
***
如果愿意看~~充字数~~的更新记录可以点[这里](#history)
***

## <span id="history">版本更新记录</span>
---

2018.11.28 v1.6.4 => v1.6.7
---
*	修改 `include/time`
*	新增 `type_onlyarray`，自定义函数可以更容易的验证类型 => **v1.6.5**
*	`BaseFunction` 内嵌版本号，`import` 由原先的硬链接改为软链接，暂时弃用 `[.vya.wav]` => **v1.6.6**
*	新增 `[object].get_index` ，可以提取 `[object]` 的索引 => **v1.6.7**
*	新增 `.get_sid` `.sys.info.next_sid`
*	修复 `.debug.list` 不能正确显示 `null` 类型的问题

2018.11.24 v1.6.4
---
*	新增 `include/time` ，可以简单地处理时间问题，修复跳过 `"\\"` `'\\'` 引起的 bug

2018.11.20 v1.6.3 => v1.6.4
---
*	`.store_data` `.load_data` 现在支持内部 `refer` 类型，但储存时间相比原先增加了一个数量级
*	新增 `.sys.limit.data_stfast`

2018.11.16 v1.6.2 => v1.6.3
---
*	新增函数 `.store_data` `.load_data` 用于变量与数据文件相互转换 => **v1.6.3**
*	修复运算 `array[i]@array` 时的错误

2018.11.13 v1.6.1 => v1.6.2
---
*	在 `README.md` 添加了一些东西和版本更新历史，让 `README` 变得稍微有那么点价值 :)
*	新增函数 `.debug.szvmat` 用来获取 `vmat` 的 `avl` 数组大小
*	函数 `.length` 可以返回 `vmat` 类型变量的子项数量
*	修复 `vmat` 扩展时有可能导致的变量被重复链接问题， `vmat_init_size` 调整到 `16` ， `object["name"]` 将不会搜索 `.gobj.*`
*	新增函数 `init.chenv` 用来改变当前变量环境，新增变量 `.sys.path.root` ，`import` `include` `data` 的生成移至 `init.math` => **v1.6.2**

2018.11.12 v1.6.0 => v1.6.1
---
*	更改变量结构 `vmat` ，现在 `vmat` 支持大量子项时的快速查找，优化?了软链接`refer`的效率 => **v1.6.1**
*	新增变量 `.sys.info.clocks_per_sec` 用来表示 `.time.clock` 的返回值尺度

2018.11.03 v1.6.0
---
*	更改 `init.auth` 函数的参数结构，使 `init.math` 可以更高效的更改变量权限
*	删除 `doc/key.txt` ，相关信息可以在 `func.sv` 中找到
*	扩展函数 `[object].exist` 功能， `exist` 可以接受数字索引来判断子项类型
*	修复函数 `[object].exist` 在传入空字符串做变量路径时的返回值类型错误
*	`include/` 进行了相关更改，添加了一些~~试用~~头文件 `:3`

2018.11.02 v1.5.7 => v1.6.0
---
*	新增初始化脚本功能，将脚本去空白符硬编译到可执行程序内，可以一定程度上自定义解释器初始化事项
*	增加配套工具 `init-makec`
*	增加 `init.sv` 以及 `init.auth` 用以在初始化脚本阶段使用
*	`.include.inc` 由 `func.sv` 移至 `init.math` 中定义
*	`init.math` 中新增函数 `.include.version` 用以便捷生成版本号结构
*	新增函数 `.cpcode` 用来移除 `{code}` 字符串中无用的空白符
*	新增 `doc/math.lang` 用来配置 `gedit文本编辑器` 的 `math脚本` 高亮显示

2018.10.30 v1.5.5 => v1.5.7
---
*	新增函数 `.ftoi` 用以将单精度浮点数转换成储存形式 => **v1.5.6**
*	为 `func.sv` 添加注释， `.type` 更改为 `vmat` 类型， `sv-builf` 增加调试模式 `--debug` ，新增函数 `.file.write` ， `.include.inc` 更改参数结构以支持自定义使用类型 => **v1.5.7**

2018.10.29 v1.5.0 => v1.5.5
---
*	新增权限 `auth_system` 标识系统内置变量，被标识变量禁止添加删除子项操作，.sv 文件内用 `c` 表示
*	修复一些权限问题，新增函数 `.thread.self` 用以获取当前线程 `pthid`
*	可以更加便捷灵活地配置内置 `.sys.path.*` => **v1.5.1**
*	新增函数 `.file.size` `.file.read` 用以文件操作，放宽了默认数组大小限制和文本文件载入限制 => **v1.5.2**
*	新增 `.sys.version.value` 用来比较版本 => **v1.5.3**
*	修复 `[object].add` `[object].replace` 增加变量时 `unsign` 属性丢失 => **v1.5.4**
*	`.debug.list` 中对数字索引子项名称的显示由 `(index)` => `[index]`
*	新增关键字 `.arrstring` 用来 `byte[]` => `string` => **v1.5.5**
*	新增 `.ltof` 函数用来将 浮点数的储存类型`uint(u32)` 双精度浮点数的储存类型`ulong(u64)` 转换成 双精度浮点数`float(double)`
*	版本号++ :)

2018.10.28 v1.4.0 => v1.5.0
---
*	`object` 类型可以接受数组索引，新增函数 `[object].add` `[object].replace` `[object].sub` 用以更灵活的新增删除变量
*	完善了 `.debug.list` 的功能，可以列出特殊的内部数据 `vlist 链表`，相应的放宽了一些系统变量的权限
*	删除了上次没有删掉的 `include/randseq.math`
*	更人性化的索引查找，现在 `object["index"]` 或 `object[index]` 如果 `index` 不存在将会返回 `type_null` 而不是 `type_err`
*	修复了一些不起眼的 bug => **v1.5.0**
*	增加了命令提示 ... (嗯，虽说等于没有罢了 orz)

2018.07.30 v1.3.2 => v1.4.0
---
*	修改了很多很多
*	自定义函数运行时会创建变量 `_caller_`，用以硬链接调用者当前环境
*	新增函数 `.issame` 用以判断不同名称的变量是否为同一个
*	新增函数 `.ftol` 用以使双精度浮点数转换成储存值
*	新增变量 `.type.all` -> `type_all`
*	新增函数 `.time.stamp` `.time.clock`
*	新增自定义函数 `.include.inc` 用以方便头文件判断是否已经运行过
*	修复关键字 `include` 的一些问题
*	新增 `include/randrom`
*	修改 `include/randseq.math` 并改名 `randseq`
*	.sv 文件支持新类型 `fvlist`，并且支持预定义数组

2018.06.11 v1.3.2
---
*	新增 `include/randseq.math` 用以根据种子和方式产生特定随机数列

2018.06.10 v1.2.1 => v1.3.2
---
*	新增函数 `.strlen` `.strbyte` 关键字 `.strarry`
*	减少了一些没必要的跳转，修复了一些内存泄漏问题
*	扩充了函数 `[object].exist` 的功能，`exist` 函数现在可以判断变量类型
*	新加引用池，增加引用类型 `type_refer`，可用 `var refer:name=var;` 定义 `name` -> `var` 的引用 => **v1.3.1**
*	放宽自定义函数运行时的权限需求，自定义函数可以在执行中新增当前环境下变量 => **v1.3.2**

2018.06.09 v1.1.0 => v1.2.1
---
*	函数 `import` 增加路径处理功能，新增 `.sys.path.*`
*	默认函数传递参数禁用数组类型，可以添加 `type_allowarray` 允许该变量位传递数组类型
*	修复在获取字符串数组元素时的返回值异常
*	新增关键字 `[object].include` 用于运行其他脚本文件 => **v1.2.1**
*	修复函数 `import` 产生变量的同名与变量名称合法性问题

2018.05.20 v1.1.0
---
*	运算符 `+` 增加字符串链接功能，增加 `.s` 为 `.sprint` 短名(硬链接)
*	减少了关键字 `delete` 对父节点权限的要求
*	新增关键字 `[object].run` 用于运行字符串脚本，增强实时创建/更改脚本的能力
*	新增关键字 `[object].try` 用于尝试运行和调试错误，同时新增系列函数 `.try.*` 用于处理抛出的错误

2018.05.19 v1.1.0
---
*	新增 `auth_relength` 权限用以细分权限可重定义数组长度，更改定义变量时的权限判定，重新设定 `.args.*` 的权限
*	添加新类型 `.type.func`，新增权限标志 `S` 在 .sv 文件用以映射 `auth_relength`
*	扩展定义变量的方式，支持 `var type:(name = *, ...), ...;`
*	在基础类型计算中添加字符串与数字之间的赋值，重写 `@` 运算符，支持字符串数组的赋值
*	更新 `example/args.math` 的例子

2018.04.01 v1.1.0
---
*	新增类周期波节获取函数 `[vya.wav].spwav_get`

2018.03.31 v1.1.0
---
*	修复函数 `[vya.wav].new` 的权限问题
*	删掉一些多余的东西 :(
*	新增内部函数 `var_link`
*	修复内置函数 `type_check` 的返回类型(u32 => s32) orz
*	更改 `.debug.list` 函数，尽可能消除了 inode 因为传参的影响
*	新增函数 `[vya.wav].spwav_new` 用于产生类周期波对象

2018.03.27 v1.1.0
---
*	修复关键字 `detele` 对非法命名变量的报错错误问题
*	修复随机数发生器上限问题，`[0,rand_max)`
*	新增函数 `[vya.wav].aloud` 求样本平方平均值

2018.03.24 v1.1.0
---
*	修复定义数组时当长度为变量或运算式时引起的定义错误
*	在 `[vya.wav]` 包中新增测噪函数 `noise`


2017.12.09 v1.1.0
---
*	修复获取 `{code}` 时产生的 bug -_-!!
*	新增函数 `.sprint`

2017.12.04 v1.1.0
---
*	新增函数 `.get_type`
*	删除 `output/` `package/model/` 目录中的占位符 `exist`
*	新增 `output/` `package/model/` 目录中的占位符 `.exist`
*	新增函数 `[vya.wav].new`
*	修复更改同名变量类型时新变量权限丢失
*	新增 `doc/key.txt` 文件作为关键字用法的说明

2017.12.03 v1.1.0
---
*	初始化框架
*	修复警告在 `(char *)=(const char *)`

