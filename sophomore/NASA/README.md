# NASA作業


## 簡介
NASA畢竟是門只有作業重要的課，這裡就整理了整個學期的作業和題目，目的是希望以後需要時可以回來翻找需要的知識。因為檔案大多為pdf及word docx，因此大部分內容都在 __[google drive](https://drive.google.com/file/d/1h_dW_L1--tKyxg2FtuaqT3xnkSaXVJnj/view?usp=sharing)__ ，github就純粹放些code。


## 各作業內容簡介

### HW0 
主題：名詞解釋、認識VM

內容：做為進入NASA的門票，NA詢問了許多關於網路協定與網路IP相關的指令使用，SA則要我們在arch VM中學習使用各種指令設定系統。

關鍵字：nslookup、tracert、nmap、telnet、Arch、pacman、ssh、port forwarding、hostname、usermod、useradd、groupadd、cowsay、sed、tr、grep、find、kill、cron、cron.d/minute

### HW2
主題：shell script

內容：單純的shell script，檢查輸入參數，並比較兩個資料夾的差異和相似度。語法問題主要參考 __[這篇](https://devhints.io/bash)__  。正確性有待商榷，雖然public是全AC，但不確定private結果，可能等以後作業成績公布再來看看。

關鍵字：shell script、diff、getopts、binary file

### HW3
主題：partition

內容：照著lab的教學，練習切硬碟、建立RAID(看要RAID0或RAID多少)、對disk加密、熟悉VG、PV、LV、LVM的架構，然後練習file system的formatting和mounting。雖然最後的Arch installation沒做出來，但有信睿電神的做法，先提供參考，以後有需要再來看。

關鍵字：partition、lsblk、fdisk、mdadm、cryptsetup、luksAddKey、luksdDump、dd、pvcreate、vgcreate、lvcreate、vgs、lvs、mkfs、mount、Arch

### HW4
主題：Cisco

內容：這次的作業有超多油圖和油小說和這門課最簡單的網路接線設定。題目沒有提的是，這次內容要用Cisco Packet Tracer完成，因為其指令的特殊性，在這裡就不贅述，反正直接進檔案裡面Ctrl+F即可。

關鍵字：cisco

### HW5
主題：Virsh、Docker、Kubernetes

內容：從這裡開始就常常很難寫完作業。前面的Virsh就是練習在工作站上建立VM，並且使用RealVNC Viewer來在Windows上遠端連線到工作站的VM；中間的Docker就是練習將docker hub上nginx image的container以docker執行，Application則是透過Docerfile在虛擬機中部署一個mysql+phpmyadmin的服務。然而後面的Kubernetes(K8s)我只完成了簡介K8s的部分，而Application，也就是部署nginx+mysql+phpmyadmin的部分我就死去了。參考 __[信Rape](https://chenhsinjui.github.io/)__ 施捨給本笨蛋的版本吧(檔案在雲端，這連結是連到信Rape的Blog)。

關鍵字：virsh、virt-install、vncdisplay、virsh console、serial-getty、qemu-monitor-command、--hmp、hostfwd_add、(hostfwd_remove)、docker、docker compose、K8s、phpmyadmin、nginx

### HW6
主題：