
# 📝 AWS Webアプリケーション設計書（詳細版）

## 1. はじめに（目的）

本ドキュメントは、React + FastAPI で構築されたWebアプリケーションをAWSに移行する際の設計内容を詳細に記述したものである。システムの可用性・拡張性・セキュリティ・保守性を考慮し、クラウドネイティブなアーキテクチャを採用している。

---

## 2. システム構成概要

| コンポーネント     | AWSサービス構成                                      | 説明                              |
|------------------|--------------------------------------------------|----------------------------------|
| フロントエンド     | React + S3 + CloudFront + Route 53               | 静的ファイルのホスティングとCDN配信、カスタムドメイン適用 |
| 認証認可         | Amazon Cognito（ユーザープール & IDプール）          | 認証・認可およびJWTトークンの発行管理         |
| API管理         | API Gateway（REST）                               | フロントエンドとLambda間の通信管理            |
| バックエンド       | AWS Lambda（FastAPI + Mangum）                    | サーバーレスでFastAPIを実行                 |
| データベース      | Amazon DynamoDB                                  | 非リレーショナルなキーバリューストレージ           |
| ログ・監視       | CloudWatch Logs                                  | アプリケーションのログ記録およびアラート設定       |
| CI/CD          | AWS CodePipeline + CodeBuild（今後導入予定）       | 継続的インテグレーションとデプロイ              |

---

## 3. フロントエンド構成（React）

### 3.1 画面構成・ルーティング

| パス          | コンポーネント            | 説明                     |
|--------------|-----------------------|------------------------|
| `/`          | `HomePage`            | トップページ               |
| `/login`     | `LoginPage`           | ログイン画面               |
| `/register`  | `RegisterPage`        | 新規登録画面               |
| `/dashboard` | `DashboardPage`       | 認証後のユーザー画面           |
| `/profile`   | `UserProfilePage`     | プロフィール編集画面           |

### 3.2 Amplify使用例

```js
import { Auth } from 'aws-amplify';

const signIn = async (username, password) => {
  try {
    const user = await Auth.signIn(username, password);
    console.log('ログイン成功:', user);
  } catch (error) {
    console.error('ログイン失敗:', error);
  }
};
```

---

## 4. バックエンド（FastAPI on Lambda）

### 4.1 デプロイ構成

- `Mangum`でFastAPIをLambda互換に変換
- `requirements.txt`を含むディレクトリをZIP化し、Lambdaにアップロード
- API Gatewayとの接続にはLambda Proxy統合を使用

### 4.2 API仕様一覧（例）

| メソッド | パス              | 説明               | 認証要否 | 備考 |
|---------|------------------|------------------|--------|------|
| GET     | `/api/profile`   | プロフィール取得       | 必要     | JWT必要 |
| POST    | `/api/submit`    | 入力データ登録         | 必要     |        |
| GET     | `/api/public`    | 公開データ取得         | 不要     |        |

---

## 5. Amazon Cognito設定

### 5.1 ユーザープール構成

- MFA: 無効（将来的に導入検討）
- パスワードポリシー: 英数字＋記号、最小8文字
- トリガー: 無（カスタムトリガー未使用）

### 5.2 トークン設定

| 種類      | 内容                                |
|----------|-------------------------------------|
| IDトークン  | ユーザー情報を含む、クライアント認証で使用      |
| Accessトークン | API認証時にAuthorizationヘッダーに付与   |
| Refreshトークン | トークン再取得用、セキュリティ対策が必要         |

---

## 6. DynamoDB テーブル設計（例）

| テーブル名      | 主キー                | 属性例                      |
|--------------|---------------------|---------------------------|
| `UserData`   | `user_id` (PK)      | `email`, `created_at`, `settings` |
| `FormData`   | `submission_id` (PK) | `user_id`, `form_data`, `timestamp` |

- `user_id` は Cognito の sub UUID を使用

---

## 7. Lambda関数設計（詳細）

### `auth_handler.py`

- Cognitoトークンからユーザー情報を抽出
- DynamoDBから該当データを取得し返却

### `submit_form.py`

- フォーム内容をJSONで受け取り、整形して保存
- データ検証は `pydantic` を利用

---

## 8. セキュリティ設計

- HTTPS強制（CloudFront + ACM証明書）
- CORS対応：API GatewayレベルでOrigin制限
- IAM最小権限ポリシー
- APIキー制限（必要時）
- CloudWatchアラート通知設定（500系エラー閾値超過）

---

## 9. 今後の開発予定

- 自動CI/CD導入（CodePipeline + GitHub連携）
- CloudWatch Logs Insightsによる可視化
- Route53ヘルスチェックによる冗長性強化

---

## 10. 添付資料

- OpenAPI（Swagger）形式のAPI仕様
- ER図（DynamoDBテーブル相関図）
- Terraform構成ファイル（今後導入予定）
