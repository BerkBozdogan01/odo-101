# ODO-101 — Sıfırdan Odometri

BURST (Boğaziçi Üniversitesi Robotik Sistemler Takımı) Otonom Sürüş
Birimi'nin 6 haftalık odometri eğitim programı için yazdığım çözümler.

**Kural:** Her satır elle yazılıyor. Harici kütüphane yok — matris
çarpımı da, Kalman filtresi de sıfırdan C++ ile.

## Neden bu proje

Odometri, çıktısına bakarak doğru olup olmadığını anlayamadığın bir
problem. Ekranda `x = 143.2` yazar; bu 20 metre kaymış da olabilir,
kusursuz da. İkisi de aynı görünür. Kayma sessizdir, bir yerde hata
verip patlamaz — usulca birikir.

O yüzden her görevde sadece kod yok, hatanın ölçümü de var.

## Durum

**Hafta 1 tamamlandı.** Hafta 2'nin kod kısmına geçildi.

| Görev | Konu | Durum |
|-------|------|-------|
| 0 | Vec2, Pose2D, açı normalizasyonu + birim testler | tamamlandı |
| 1 | Enkoder simülatörü + ölü hesap (Euler vs. tam yay) | devam ediyor |
| 2 | Ackermann odometrisi, kalibrasyon hatası deneyi | bekliyor |
| 3 | Monte Carlo, kovaryans elipsi | bekliyor |
| 4 | Elle Mat3 + Kalman/EKF | bekliyor |
| 5 | Enkoder + jiroskop füzyonu | bekliyor |
| Bonus | ROS 2 / AWSIM entegrasyonu | bekliyor |

## Derleme

Her görev kendi klasöründe, tek dosya:

    g++ -std=c++17 -Wall -Wextra main.cpp -o gorevN
    ./gorevN

## Yapı

Her `gorevN/` klasöründe kodun yanında bir `README.md` var: nasıl
derlenir, ne çıkarır, o görevde ne öğrendim.
