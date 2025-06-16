# ✅ React + AWS Cognito ログイン後ページ遷移付き実装（日本語）

---

## 🔧 機能概要

- AWS Cognito によるログイン機能
- JWT トークンを取得
- トークンを使ってバックエンド API を呼び出し検証
- 成功時に /home ページへリダイレクト

---

## 📄 Login.tsx（完全なコンポーネント）

```tsx
import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';
import { Amplify, Auth } from 'aws-amplify';
import axios from 'axios';

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

      setStatus("✅ ログイン成功、リダイレクト中…");

      await axios.get("https://your-api-id.execute-api.ap-northeast-1.amazonaws.com/history", {
        headers: { Authorization: `Bearer ${token}` }
      });

      navigate("/home");
    } catch (err) {
      console.error(err);
      setStatus("❌ ログイン失敗：" + err.message);
    }
  };

  return (
    <div style={{ padding: 20, maxWidth: 400 }}>
      <h2>🔐 ログイン</h2>
      <input type="email" placeholder="メールアドレス" value={email} onChange={(e) => setEmail(e.target.value)} />
      <input type="password" placeholder="パスワード" value={password} onChange={(e) => setPassword(e.target.value)} />
      <button onClick={handleLogin}>ログイン</button>
      <p>{status}</p>
    </div>
  );
};

export default Login;
```

---

## 🛣️ React Router 構成（App.tsx）

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

## 🆕 Home.tsx サンプル

```tsx
const Home = () => {
  return (
    <div style={{ padding: 20 }}>
      <h1>🎉 ホームページへようこそ！</h1>
    </div>
  );
};

export default Home;
```

---

## 📦 インストールすべき依存パッケージ

```bash
npm install aws-amplify axios react-router-dom
```

---

# 完了 ✅