# ✅ AWS Cognito × API Gateway × React 手動設定ガイド

---

## 🧱 ステップ1：Cognitoユーザープールの作成

1. AWSコンソールにログイン → 「Cognito」で検索
2. 「ユーザープール」→ 「ユーザープールを作成」

### 設定：
- ユーザープール名：SearchUserPool
- サインイン方法：メールアドレス
- パスワードポリシー：デフォルト
- メール認証：Cognitoでメール送信を有効化

> 作成後に **User Pool ID** と **Region** を控える

---

## 🧱 ステップ2：アプリクライアントの作成

1. ユーザープール内「アプリクライアント」→「作成」
2. 設定：
   - 名前：search-web
   - クライアントシークレット生成：無効
   - OAuth2 有効化
   - コールバックURL：CloudFrontなどのフロントURL
   - OAuth Flow：Authorization code grant を有効
   - OAuth Scope：openid, email を有効

> 作成後に **App Client ID** を控える

---

## 🔐 ステップ3：API Gateway に Cognito オーソライザーを設定

1. API Gateway を開く
2. 対象API を選択（REST または HTTP）
3. 左の「オーソライザー」→「作成」

### 設定：
- タイプ：Cognito
- ユーザープール：作成済みのものを指定
- トークンソース：Authorization（デフォルト）

---

## 🚪 ステップ4：エンドポイントにオーソライザーを適用

1. `/search` と `/history` のメソッド（GET/POST）を選択
2. 「認証」→ 「Cognitoオーソライザー」を選択
3. 「認証必須」にチェックを入れる

---

## 🖥️ ステップ5：React フロントエンドに統合

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

// ログイン
await Auth.signIn(email, password);

// JWTトークン取得
const token = (await Auth.currentSession()).getIdToken().getJwtToken();
```

---

## 🔗 ステップ6：API呼び出し時にトークンを付与

```ts
axios.get("/history", {
  headers: { Authorization: `Bearer ${token}` }
});
```

---

## 📝 注意点

| 項目 | 説明 |
|------|------|
| トークン形式 | Bearer トークン |
| 有効期限 | デフォルトは1時間 |
| デバッグ | PostmanなどでJWT付きのテスト可 |