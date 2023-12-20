arquivo="/etc/dhcpcd.conf"
sudo sed -i '/interface wlan0/s/^#//' $arquivo
sudo sed -i '\|^#     static ip_address=IP/24| s|^#||' $arquivo
sudo sed -i '/nohook wpa_supplicant/s/^#//'  $arquivo

sudo service dhcpcd restart

sudo systemctl start dnsmasq
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
sudo systemctl start hostapd

sudo reboot
