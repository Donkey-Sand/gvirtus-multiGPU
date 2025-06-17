在 API Gateway 中设置 CORS（手动方法）
对于 REST API：
	1.	打开 API Gateway → 找到对应的 API
	2.	选择某个路径（如 /search），展开方法（POST）
	3.	点击「Method Response」
	•	添加一个 response header：Access-Control-Allow-Origin（必需）
	4.	返回到「Integration Response」
	•	添加头部映射：将 Lambda 的响应中的 Access-Control-Allow-Origin 设置为 ' * ' 或指定域
	5.	对于每个路径还需要添加 OPTIONS 方法（浏览器预检请求）

建议响应头设置如下：
Header
示例值
Access-Control-Allow-Origin
* 或 https://example.com
Access-Control-Allow-Methods
GET, POST, OPTIONS
Access-Control-Allow-Headers
Content-Type, Authorization

对于 HTTP API：
	1.	进入 API Gateway → HTTP API → 选择目标 API
	2.	点击左侧「CORS」
	3.	设置如下：

 项目
示例设置
Access-Control-Allow-Origin
* 或 https://your-site.com
Access-Control-Allow-Methods
GET, POST, OPTIONS
Access-Control-Allow-Headers
Authorization, Content-Type



	4.	保存 → 部署（点右上角「Deploy」）

⸻

🧪 小技巧
	•	如果你用的是 Lambda Proxy Integration，Lambda 中响应必须手动返回 CORS headers：

return {
  "statusCode": 200,
  "headers": {
    "Access-Control-Allow-Origin": "*",
    "Access-Control-Allow-Headers": "Content-Type,Authorization",
    "Access-Control-Allow-Methods": "OPTIONS,POST,GET"
  },
  "body": json.dumps({"message": "OK"})
}

 
