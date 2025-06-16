# ✅ React + AWS Cognito 登录后跳转主页示例（中文）

---

## 🔧 功能介绍

- 使用 AWS Cognito 实现用户登录
- 登录后自动获取 JWT token
- 成功登录后调用后端 API 测试 token 有效性
- 登录成功后自动跳转到首页（/home）

---

## 📄 Login.tsx（完整组件）

```tsx
import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { Amplify, Auth } from 'aws-amplify';
import axios from 'axios';

// Cognito 配置（替换为实际值）
Amplify.configure({
  Auth: {
    region: 'ap-northeast-1',
    userPoolId: 'ap-northeast-1_xxxxxxxx',
    userPoolWebClientId: 'xxxxxxxxxxxxxxxxxxxxxxxxxx',
    mandatorySignIn: true
  }
});

const Login = () => {
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [status, setStatus] = useState('');
  const navigate = useNavigate();

  const handleLogin = async () => {
    try {
      await Auth.signIn(email, password);
      const session = await Auth.currentSession();
      const token = session.getIdToken().getJwtToken();

      setStatus("✅ 登录成功，正在加载主页…");

      await axios.get("https://your-api-id.execute-api.ap-northeast-1.amazonaws.com/history", {
        headers: { Authorization: `Bearer ${token}` }
      });

      navigate("/home");
    } catch (err) {
      console.error(err);
      setStatus("❌ 登录失败：" + err.message);
    }
  };

  return (
    <div style={{ padding: 20, maxWidth: 400 }}>
      <h2>🔐 登录</h2>
      <input type="email" placeholder="邮箱" value={email} onChange={(e) => setEmail(e.target.value)} />
      <input type="password" placeholder="密码" value={password} onChange={(e) => setPassword(e.target.value)} />
      <button onClick={handleLogin}>登录</button>
      <p>{status}</p>
    </div>
  );
};

export default Login;
```

---

## 🛣️ React Router 配置（App.tsx）

```tsx
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import Login from './components/Login';
import Home from './components/Home';

function App() {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<Login />} />
        <Route path="/home" element={<Home />} />
      </Routes>
    </Router>
  );
}
```

---

## 🆕 Home.tsx 示例

```tsx
const Home = () => {
  return (
    <div style={{ padding: 20 }}>
      <h1>🎉 欢迎来到首页！</h1>
    </div>
  );
};

export default Home;
```

---

## 📦 依赖安装

```bash
npm install aws-amplify axios react-router-dom
```

---

# 完成 ✅