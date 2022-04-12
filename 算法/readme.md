### vscode编译器添加c++万能头文件
https://www.csdn.net/tags/MtTaEg1sMDkzNzgxLWJsb2cO0O0O.html


### vscode的用户代码片段  
代码片段是一些使用率极高的代码，开发的过程中可能会频繁使用到，如果通过设置编辑器或者IDE的Snippets自动完成代码，可以显著地提高开发的效率。  
比如刷算法的时候经常需要在文件中添加vector、iostream、algorithm、math等库文件和using namespace std;代码。  
使用代码片段（Code Snippets）可以快速添加这些代码。  
https://zhuanlan.zhihu.com/p/357377511  

在文件中添加如下代码：
```cpp
{
	"auto add header":{
		// "scope":"c,cpp",
		"prefix": "header",
		"body": [
			"#include <vector>",
			"#include <iostream>",
			"#include <algorithm>",
			"using namespace std;",
		],	
		"description": "auto add commom algorithm header"
	}
}
```
详细攻略请看链接。
