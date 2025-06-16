# ✅ React + Cognito ログイン構成説明（日本語）

---

## 📁 プロジェクト構成

```
my-app/
├── public/
│   └── index.html
├── src/
│   ├── components/
│   │   ├── Login.tsx        ← ログインページ（リダイレクト付き）
│   │   └── Home.tsx         ← ログイン後の画面
│   ├── App.tsx              ← Reactルーティング設定
│   ├── index.tsx            ← エントリーポイント
├── package.json             ← 依存パッケージ設定
```

---

## 🚀 プロジェクト作成＆依存インストール

```bash
npx create-react-app my-app --template typescript
cd my-app
npm install aws-amplify axios react-router-dom
```

---

## 🧩 Login.tsx の説明

- Amplify を使って Cognito にサインイン
- JWT トークンを取得し API に送信
- 成功後、`/home` に自動遷移

---

## 🛣️ ルーティング（App.tsx）

- `/`：ログイン画面
- `/home`：ログイン成功後の画面

---

## 🖥️ Home.tsx

- シンプルな歓迎画面
- 将来的に検索履歴やユーザー情報を表示可能

---

## 🛠️ index.tsx

- React の起点ファイル

---

## 🧪 起動方法

```bash
npm start
```

ブラウザで `http://localhost:3000/` にアクセスしてログインテスト開始。

---

## 🔧 修正が必要な箇所

- Cognito の region、User Pool ID、App Client ID
- API Gateway のエンドポイント

---

## ✅ 正常動作の確認項目

- メール＋パスワードでログインできる
- 成功すると `/home` に遷移
- API から取得した検索履歴などを表示可能

---