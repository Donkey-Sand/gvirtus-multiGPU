

当然，Jo。下面是你想要的**公司子域名设置 + CloudFront 绑定的完整流程**，适用于你想使用例如 `ai-methon.an.shimadzu.co.jp` 作为你程序的网址。

---

## ✅ 子域名设置 + CloudFront 绑定的流程

---

### 🧩 步骤 1：公司 IT 管理员设置子域名（DNS 配置）

让负责 `shimadzu.co.jp` 或 `an.shimadzu.co.jp` 的 DNS 管理员：

1. **新建子域名**（例如）：

   ```
   ai-methon.an.shimadzu.co.jp
   ```
2. **添加一条 CNAME 记录**：

   ```
   名称（Host）：ai-methon.an.shimadzu.co.jp  
   类型：CNAME  
   值（Value）：dxxxxxxxxxxxx.cloudfront.net（你的 CloudFront 域名）
   TTL：300 或默认值
   ```

---

### 🧩 步骤 2：申请 SSL 证书（ACM）

在 AWS 控制台的 **ACM（AWS Certificate Manager）** 中申请该域名的 SSL 证书：

1. 打开 ACM 控制台（选择 **バージニア北部（us-east-1）** 区域）
2. 点击「Request a certificate」
3. 输入域名：

   ```
   ai-methon.an.shimadzu.co.jp
   ```
4. 选择 **DNS 验证（DNS validation）**
5. 系统会给出一个 CNAME 记录 → 提供给公司 DNS 管理员添加

✔️ 等待 DNS 验证通过，ACM 证书状态会变成 “Issued”。

---

### 🧩 步骤 3：CloudFront 配置自定义域名

在 CloudFront 控制台：

1. 选择你的分发（Distribution） → 点击「Edit」
2. 在 **Alternate domain names (CNAMEs)** 中添加：

   ```
   ai-methon.an.shimadzu.co.jp
   ```
3. 在 **SSL certificate** 部分，选择刚刚申请成功的 ACM 证书

---

### 🧩 步骤 4：测试访问

等待 DNS 生效（可能需要几分钟～数小时），然后访问：

```
https://ai-methon.an.shimadzu.co.jp
```

即可看到你部署的 CloudFront 前端页面。

---

## ✉️ 向 IT 管理员申请子域名设置（日文邮件模板）

以下是你可以发送的申请邮件：

---

### 📩 件名：

CloudFront 利用のためのサブドメイン（ai-methon.an.shimadzu.co.jp）設定のお願い

### 📩 本文：

```
お疲れ様です。ジョです。

AWS CloudFront にて社内システムのフロントエンドを公開するため、
以下のサブドメインの設定をご対応いただきたくご連絡いたしました。

■ 追加したいサブドメイン：
ai-methon.an.shimadzu.co.jp

■ ご対応いただきたい内容：
1. DNS にて以下の CNAME レコードを追加：
   ホスト名：ai-methon.an.shimadzu.co.jp  
   値：dXXXXXXXXXX.cloudfront.net（CloudFront ドメイン）

2. SSL証明書用の検証用 CNAME レコードも追加予定（別途連絡いたします）

ご多忙のところ恐れ入りますが、何卒よろしくお願いいたします。
```

---

如需我帮你将具体的 CloudFront 域名或 ACM 验证记录写进信里，也可以告诉我 👍


もちろん、Jo。以下に「子ドメインの設定 + CloudFront 連携の手順」を日本語で整理しました。

---

## ✅ 子ドメイン設定 + CloudFront 連携手順（日本語版）

---

### 🧩 手順1：会社のDNS管理者に子ドメインを設定してもらう

`shimadzu.co.jp` または `an.shimadzu.co.jp` を管理している社内のIT部門に、以下の設定を依頼してください。

1. **新しい子ドメインの作成**（例）：

   ```
   ai-methon.an.shimadzu.co.jp
   ```

2. **CNAME レコードの追加**：

   ```
   ホスト名（名前）：ai-methon.an.shimadzu.co.jp  
   種別：CNAME  
   値（宛先）：dxxxxxxxxxxxx.cloudfront.net（CloudFrontのドメイン）  
   TTL：300（もしくはデフォルト値）
   ```

---

### 🧩 手順2：AWS Certificate Manager（ACM）でSSL証明書を申請

AWSマネジメントコンソールの **ACM（証明書マネージャー）** を開き、以下のように操作します。

1. **バージニア北部（us-east-1）** リージョンを選択
2. 「証明書のリクエスト」をクリック
3. ドメイン名を入力：

   ```
   ai-methon.an.shimadzu.co.jp
   ```
4. 検証方法：**DNS検証**を選択
5. 表示される CNAME 検証レコードを DNS 管理者に共有して、DNS に追加してもらう

✔️ DNS 検証が完了すると、証明書のステータスが「発行済み（Issued）」になります。

---

### 🧩 手順3：CloudFront にカスタムドメインを設定

CloudFront コンソールで以下の操作をします：

1. 対象のディストリビューションを選択し、「Edit（編集）」をクリック
2. **Alternate domain names (CNAMEs)** に以下を追加：

   ```
   ai-methon.an.shimadzu.co.jp
   ```
3. **SSL証明書**の欄で、先ほど発行された ACM の証明書を選択

---

### 🧩 手順4：アクセス確認

DNS が反映された後、以下の URL にアクセスして確認します：

```
https://ai-methon.an.shimadzu.co.jp
```

---

## ✉️ IT部門への依頼メール（テンプレート）

---

### 件名：

CloudFront 利用のためのサブドメイン（ai-methon.an.shimadzu.co.jp）設定のお願い

### 本文：

```
お疲れ様です。ジョです。

AWS CloudFront にて社内システムのフロントエンドを公開するため、
以下のサブドメインの設定をご対応いただきたくご連絡いたしました。

■ 追加したいサブドメイン：
ai-methon.an.shimadzu.co.jp

■ ご対応いただきたい内容：
1. DNS にて以下の CNAME レコードを追加：
   ホスト名：ai-methon.an.shimadzu.co.jp  
   値：dXXXXXXXXXX.cloudfront.net（CloudFront ドメイン）

2. SSL証明書用の検証用 CNAME レコードも追加予定（別途連絡いたします）

ご多忙のところ恐れ入りますが、何卒よろしくお願いいたします。
```

---

必要であれば、CloudFrontのドメイン名やACM検証レコードを記入した状態のメールテンプレートも作成しますので、お気軽にお申し付けください。
