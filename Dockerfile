FROM python:3.8-slim-bullseye

# Install required base packages
RUN apt-get update && \
    apt-get install -y --no-install-recommends wget openssl ca-certificates vim git binutils libc6 libstdc++6 && \
    rm -rf /var/lib/apt/lists/*

# (Optional) Download and extract xtensa toolchain for aarch64 if OTA builds are needed
# RUN wget https://github.com/koendv/xtensa-esp32-elf-raspberrypi/releases/download/v8.4.0-2020r3/xtensa-esp32-elf-linux-aarch64-2020r3.tar.gz && \
#     tar -xzf xtensa-esp32-elf-linux-aarch64-2020r3.tar.gz -C /opt && \
#     rm xtensa-esp32-elf-linux-aarch64-2020r3.tar.gz
# ENV PATH="/opt/xtensa-esp32-elf/bin:${PATH}"


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

#
