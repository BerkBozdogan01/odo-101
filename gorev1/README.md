# Görev 1 — Enkoder Simülatörü + Ölü Hesap
Sabit v ve w ile daire çizen araç; enkoder tick kuantalaması (tam sayı!).
Tick'lerden poz geri kurma: Euler vs. tam yay entegrasyonu.
Çıktı: 100 tur sonunda hata-zaman CSV'si + grafik.

## Hafta 2 — kısa özet

Enkoder mesafe değil tick sayar: kuadratür, ölçek sabiti, kuantalama ve
kalanın devri ("dürüst simülatör"). Diferansiyel sürüş kinematiği yay
formülünden türetiliyor; ölü hesap tanımlanıp Hafta 1'in `compose`'u iç
döngüye giriyor. Euler ile tam yay entegrasyonunun varsayım ve hata
mertebesi farkı bu görevde ölçülecek. Tam anlatım:
[ders_notlari/hafta2_konu_anlatimi.pdf](../ders_notlari/hafta2_konu_anlatimi.pdf)
