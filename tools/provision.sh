#!/bin/bash
set -euo pipefail
export DEBIAN_FRONTEND=noninteractive
test ! -f /root/laqia-server-prepared || { echo 'Server already initialized'; exit 0; }
dpkg --add-architecture i386
apt-get update -qq
apt-get install -y -qq mariadb-server libc6:i386 libstdc++6:i386 libgcc-s1:i386 libcrypt1:i386 zlib1g:i386 zlib1g-dev:i386 g++-multilib libmariadb-dev gdb iptables
mkdir -p /opt/laqia/legacy /opt/laqia/server /opt/laqia/gm-build /opt/laqia/charsets
dpkg-deb -x /root/laqia-setup/libmysqlclient15.deb /opt/laqia/legacy
printf '[mysqld]\nsql_mode=\nbind-address=127.0.0.1\ncharacter-set-server=latin1\ncollation-server=latin1_swedish_ci\n' > /etc/mysql/mariadb.conf.d/99-laqia-local.cnf
systemctl restart mariadb
if ! test -f /root/laqia-db-initialized; then
    mariadb < /root/laqia-setup/schema.sql
    mariadb < /root/laqia-setup/local-account.sql
    touch /root/laqia-db-initialized
fi
tar xzf /root/laqia-setup/server.tar.gz -C /opt/laqia/server
tar xzf /root/laqia-setup/build.tar.gz -C /opt/laqia/gm-build
mkdir -p /opt/laqia/server/LAQIA_LoginServer/ALLOW_ID /opt/laqia/server/LAQIA_LoginServer/DENY_ID
touch /opt/laqia/server/LAQIA_LoginServer/ALLOW_ID/developer
id laqia >/dev/null 2>&1 || useradd --system --home /opt/laqia --shell /bin/bash laqia
chown -R laqia:laqia /opt/laqia
for srv in LAQIA_DataServer LAQIA_GameServer LAQIA_LoginServer; do
    chmod +x /opt/laqia/server/$srv/$srv
    cp /root/laqia-setup/$srv.service /etc/systemd/system/$srv.service
    # MySQL 5.0's XML parser crashes on modern MariaDB charset definitions.
    # Use built-in legacy charsets in these services; leave MariaDB's files intact.
    mkdir -p /etc/systemd/system/$srv.service.d
    printf '[Service]\nBindReadOnlyPaths=/opt/laqia/charsets:/usr/share/mysql/charsets\n' > /etc/systemd/system/$srv.service.d/legacy-charsets.conf
    LD_LIBRARY_PATH=/opt/laqia/legacy/usr/lib ldd /opt/laqia/server/$srv/$srv | tee /tmp/laqia-library-check
    ! grep -q 'not found' /tmp/laqia-library-check
done
# Keep the legacy game service's outbound traffic on the local VM.
iptables -A OUTPUT -m owner --uid-owner laqia -o lo -j ACCEPT
iptables -A OUTPUT -m owner --uid-owner laqia -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT
iptables -A OUTPUT -m owner --uid-owner laqia -j REJECT
iptables-save > /etc/laqia-loopback.rules
cat > /etc/systemd/system/laqia-loopback-only.service <<'UNIT'
[Unit]
After=network.target
[Service]
Type=oneshot
RemainAfterExit=yes
ExecStart=/bin/sh -c '/usr/sbin/iptables-restore < /etc/laqia-loopback.rules'
UNIT
systemctl daemon-reload
systemctl start LAQIA_DataServer LAQIA_GameServer LAQIA_LoginServer
sleep 3
for srv in LAQIA_DataServer LAQIA_GameServer LAQIA_LoginServer; do systemctl is-active --quiet "$srv"; done
rm -f /root/laqia-setup/local-account.sql
touch /root/laqia-server-prepared
