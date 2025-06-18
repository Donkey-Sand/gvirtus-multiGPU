步骤一：准备 Docker 镜像
	1.	编写 Dockerfile
AWS 提供了基础的 Lambda 镜像，你可以在此基础上构建。例如 Python 3.11：

FROM public.ecr.aws/lambda/python:3.11

# 拷贝代码到容器
COPY app.py ${LAMBDA_TASK_ROOT}

# 安装依赖
COPY requirements.txt .
RUN pip install -r requirements.txt -t "${LAMBDA_TASK_ROOT}"

# 指定 handler（文件名.函数名）
CMD ["app.lambda_handler"]

	2.	创建本地镜像

docker build -t my-lambda-image .

✅ 步骤二：将镜像推送到 Amazon ECR
	1.	登录 ECR
 
aws ecr get-login-password --region ap-northeast-1 | \
  docker login --username AWS --password-stdin <your_account_id>.dkr.ecr.ap-northeast-1.amazonaws.com


✅ 步骤三：构建并测试本地容器（可选）

docker build -t my-lambda-image .

本地测试（安装 AWS 提供的 Lambda Runtime Interface Emulator）：
docker run -p 9000:8080 my-lambda-image

然后访问：

curl -XPOST "http://localhost:9000/2015-03-31/functions/function/invocations" -d '{}'


  

 

