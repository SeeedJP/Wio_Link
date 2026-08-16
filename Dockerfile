FROM python:3.8-slim-bullseye

# Install required base packages
RUN apt-get update && \
    apt-get install -y --no-install-recommends wget openssl ca-certificates vim git binutils libc6 libstdc++6 && \
    rm -rf /var/lib/apt/lists/*

# ESP8266 (xtensa-lx106-elf) cross-compiler toolchain, needed for OTA/driver firmware builds.
# The prebuilt arm release is a 32-bit armhf binary even on 64-bit (aarch64) hosts like a
# Raspberry Pi, so armhf multiarch libs are installed to let it run.
# Verified working source: https://github.com/esp8266/Arduino releases (tag 2.3.0).
RUN set -ex; \
    ARCH=$(dpkg --print-architecture); \
    if echo "$ARCH" | grep -q -e x86 -e amd; then \
        wget -O /tmp/xtensa.tar.gz https://github.com/esp8266/Arduino/releases/download/2.3.0/linux64-xtensa-lx106-elf-gb404fb9.tgz; \
    else \
        dpkg --add-architecture armhf; \
        apt-get update; \
        apt-get install -y --no-install-recommends \
            libc6:armhf libstdc++6:armhf libgcc1:armhf zlib1g:armhf libidn2-0:armhf libunistring2:armhf; \
        rm -rf /var/lib/apt/lists/*; \
        wget -O /tmp/xtensa.tar.gz https://github.com/esp8266/Arduino/releases/download/2.3.0/linuxarm-xtensa-lx106-elf-g46f160f-2.tar.gz; \
    fi; \
    tar -xzf /tmp/xtensa.tar.gz -C /opt; \
    rm /tmp/xtensa.tar.gz
ENV PATH="/opt/xtensa-lx106-elf/bin:${PATH}"


# Create a constraints file that forces pip to use Cython<3
RUN echo "cython<3" > /tmp/constraints.txt

# Python 3 dependencies
RUN pip install --no-cache-dir 'tornado<5' PyJWT pycryptodome tornado-cors psutil PyYAML

# Add the project files into the image
RUN mkdir -p /root/wio
WORKDIR /root/wio
COPY . /root/wio

# Scan drivers (optional but harmless)
RUN python3 ./scan_drivers.py || true

# Expose ports
EXPOSE 8000 8001 8080 8081

# Default command (overridden by docker-compose)
CMD bash
