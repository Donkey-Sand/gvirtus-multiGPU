# ✅ AWS Cognito × API Gateway × React 手动配置指南

---

## 🧱 第一步：创建 Cognito User Pool（用户池）

1. 登录 AWS 控制台 → 搜索「Cognito」
2. 选择「User pools」→ 点击「创建用户池」

### 设置：
- 用户池名称：SearchUserPool
- 登录方式：选择「电子邮件地址」
- 密码策略：默认即可
- 邮箱验证：启用「Cognito发送邮件」

> 完成后记下 **User Pool ID** 和 **Region**

---

## 🧱 第二步：创建 App Client

1. 在用户池内选择「App clients」→ 点击「创建 App client」
2. 设置：
   - 名称：search-web
   - 不勾选「生成密钥」
   - 开启 OAuth 2.0
   - Callback URL：填写前端网址（如 CloudFront 地址）
   - OAuth Flow：勾选「Authorization code」
   - OAuth Scope：勾选 openid, email

> 完成后记下 **App client ID**

---

## 🔐 第三步：在 API Gateway 中配置 Cognito 授权

1. 打开「API Gateway」
2. 找到你的 API（REST 或 HTTP）
3. 左侧选择「Authorizers」→ 「创建授权器」

### 设置：
- 类型：Cognito
- 用户池：选择刚创建的
- Token 来源：Authorization（默认）

---

## 🚪 第四步：在 API 路径中绑定 Cognito

1. 打开你的 `/search` 与 `/history` 路径
2. 点击方法（GET/POST）→ 选择「授权」
3. 指定使用的 Cognito 授权器，并勾选「强制授权」

---

## 🖥️ 第五步：React 前端中集成 Cognito

```tsx
import { Amplify, Auth } from 'aws-amplify';

Amplify.configure({
  Auth: {
    region: 'ap-northeast-1',
    userPoolId: 'ap-northeast-1_XXXXXXX',
    userPoolWebClientId: 'xxxxxxxxxxxxxxxxxxxx',
    mandatorySignIn: true
  }
});

// 登录
await Auth.signIn(email, password);

// 获取 JWT token
const token = (await Auth.currentSession()).getIdToken().getJwtToken();
```

---

## 🔗 第六步：API 调用时附带 token

```ts
axios.get("/history", {
  headers: { Authorization: `Bearer ${token}` }
});
```

---

## 📝 注意事项

| 项目 | 说明 |
|------|------|
| Token 格式 | Bearer token |
| 有效期 | 默认为1小时 |
| 控制台调试 | 可用Postman携带JWT token请求API |