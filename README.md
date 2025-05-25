# Bank Application

![](https://i.ibb.co/q3MMqg71/bank.jpg)

Bu proje, C programlama dili kullanılarak geliştirilmiş basit bir **Banka Sistemidir**. Kullanıcılar kayıt olabilir, giriş yapabilir, para yatırma/çekme işlemleri gerçekleştirebilir ve diğer kullanıcılara para transferi yapabilirler. 
Veriler `.json` formatında saklanır ve her kullanıcıya özel 6 haneli bir kullanıcı ID'si oluşturulur.

## Özellikler

- Kayıt Olma (Ad Soyad, Kimlik Numarası, Şifre)
- Giriş Yapma (Kimlik Numarası ve Şifre ile)
- Bakiye Görüntüleme
- Para Yatırma
- Para Çekme
- Kullanıcıların Sahip Olduğu Benzersiz ID İle Kullanıcılar Arası Para Transferi
- Kullanıcı Verilerini JSON dosyasında saklama

## Kurulum ve Çalıştırma

1. Projeyi klonlayın veya indirin:
```bash
git clone https://github.com/cancevik0/bank-app.git
```
2. Projeyi Çalıştırın:
```bash
gcc main.c cJson.c -o bank-app
```

# Gereksinimler

- GCC (C derleyicisi)  
- cJson  

# Geliştirici

Can Çevik
