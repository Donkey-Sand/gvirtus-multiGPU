
# 🚀 Lambda デプロイ準備手順書（ZIP & CloudFormation）

作成日：2025年6月16日  
作成者：Jo  

---

## ✅ ステップ1：Lambda ZIP パッケージの作成

Lambda で FastAPI を動かすには必要ライブラリも含めて ZIP にする必要があります。以下の手順でローカル環境にて ZIP パッケージを作成します。

### 📁 ディレクトリ構成例

```
lambda_package/
├── app/
│   ├── __init__.py
│   ├── main.py
│   └── auth.py
├── requirements.txt
```

### 🔧 手順（Mac/Linux 環境例）

```bash
cd lambda_package

# 仮想環境作成と有効化
python3 -m venv venv
source venv/bin/activate

# ライブラリをインストール（Lambda に入れる必要があるパッケージ）
pip install -r requirements.txt

# site-packages をコピー
mkdir -p build/python
cp -r venv/lib/python*/site-packages/* build/python/

# コードをコピー
cp -r app build/python/

# ZIP 作成
cd build
zip -r9 ../lambda_function.zip .
```

### 💡 注意点
- ZIP のルートに `main.py` を含める必要があります
- Lambda で `handler` は `main.handler` と設定する必要あり（AWS側設定）

---

## 📦 ステップ2：CloudFormation テンプレート（YAML）

以下は CloudFormation で Lambda + API Gateway + DynamoDB を一括デプロイする例です。

```yaml
AWSTemplateFormatVersion: '2010-09-09'
Transform: AWS::Serverless-2016-10-31
Description: 検索履歴管理用Lambdaアプリ

Resources:
  SearchHistoryTable:
    Type: AWS::DynamoDB::Table
    Properties:
      TableName: UserSearchHistory
      BillingMode: PAY_PER_REQUEST
      AttributeDefinitions:
        - AttributeName: user_id
          AttributeType: S
        - AttributeName: timestamp
          AttributeType: S
      KeySchema:
        - AttributeName: user_id
          KeyType: HASH
        - AttributeName: timestamp
          KeyType: RANGE

  SearchApiFunction:
    Type: AWS::Serverless::Function
    Properties:
      FunctionName: SearchApiFunction
      CodeUri: ./lambda_function.zip
      Handler: app.main.handler
      Runtime: python3.11
      Timeout: 30
      MemorySize: 256
      Policies:
        - DynamoDBCrudPolicy:
            TableName: UserSearchHistory
      Events:
        Search:
          Type: Api
          Properties:
            Path: /search
            Method: post
        History:
          Type: Api
          Properties:
            Path: /history
            Method: get
```

### ⏱ デプロイ手順（AWS SAM CLI を利用）

```bash
sam build
sam deploy --guided
```

---

## 🔐 補足

- Cognito JWT の検証を行うには、事前に User Pool の JWKS エンドポイントを参照するか、`python-jose` を使用して claim をパースする方法を取ります
- CloudFormation に API Gateway + Lambda のマッピングを記述することで、検索エンドポイントのデプロイが一発で可能になります

---

次にやりたいこと（例：本番環境へのデプロイ、React 側の統合、API 動作テストなど）があれば、気軽に聞いてください！
