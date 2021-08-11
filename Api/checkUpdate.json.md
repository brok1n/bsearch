# 数据模板
```
{
	"code":200,
	"msg":"success",
	"data": {
		"versionCode":20210811,
		"versionName":"1.0.0",
		"update":true,
		"files":[
			{
				"fileName":"",
				"fileMd5":"",
				"filePath":"",
				"url":"",
				"bakUrl":"",
			}
		]
	}
}
```

# 参数描述
|  字段名   | 类型 | 描述 |
|  ----  | ----  |  ----  |
| code  | int | 接口响应是否成功标志，200为成功 |
| msg  | string | 接口响应附带消息 |
| data  | Object | 接口具体数据 |
| versionCode  | int | 版本号 |
| versionName | string | 版本名 |
| update | bool | 是否更新 |
| files | Array | 该版本需要更新的文件列表 |
| fileName | string | 文件名 |
| fileMd5 | string | 文件MD5 |
| filePath | string | 文件存放文件夹名，默认当前目录则为空 |
| url | string | 文件下载地址 |
| bakUrl | string | 文件备用下载地址 |

