sudo systemctl stop hostapd

arquivo="/etc/dhcpcd.conf"
sudo sed -i '/interface wlan0/ s/^/#/' $arquivo
sudo sed -i '\|     static ip_address=IP/24| s|^|#|' $arquivo
sudo sed -i '/     nohook wpa_supplicant/ s/^/#/' $arquivo

sudo reboot
