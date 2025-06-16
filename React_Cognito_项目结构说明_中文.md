# ✅ React + Cognito 登录项目完整结构说明（中文）

---

## 📁 项目目录结构

```
my-app/
├── public/
│   └── index.html
├── src/
│   ├── components/
│   │   ├── Login.tsx        ← 登录页面（含跳转功能）
│   │   └── Home.tsx         ← 登录后跳转页
│   ├── App.tsx              ← React 路由配置
│   ├── index.tsx            ← React 入口文件
├── package.json             ← 项目配置与依赖
```

---

## 🚀 创建项目与安装依赖

```bash
npx create-react-app my-app --template typescript
cd my-app
npm install aws-amplify axios react-router-dom
```

---

## 🧩 登录组件说明（Login.tsx）

- 通过 `aws-amplify` 登录 Cognito 用户池
- 登录成功后获取 JWT Token
- 使用 Token 调用 API Gateway 接口
- 最后通过 `useNavigate` 自动跳转到 `/home`

---

## 🛣️ 路由配置（App.tsx）

- `/`：登录页面
- `/home`：登录后跳转页面

---

## 🖥️ Home 页面（Home.tsx）

- 简单显示欢迎语
- 可拓展显示用户信息、搜索结果等内容

---

## 🛠️ index.tsx

- 标准的 React 入口代码，用于挂载 App

---

## 🧪 启动项目

```bash
npm start
```

浏览器访问 `http://localhost:3000/` 开始登录测试。

---

## 🔧 修改项提醒

请替换以下字段为你项目的实际值：

- Cognito 用户池 ID、Client ID、region
- API Gateway 请求地址

---

## ✅ 成功运行后

- 登录界面可正常输入邮箱和密码
- 登录成功后会跳转到首页
- 后端返回的搜索历史也可集成展示

---