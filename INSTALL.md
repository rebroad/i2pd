# Installation Guide for i2pd

This document provides instructions for installing and building i2pd from source, including the critical reseed certificates required for network connectivity.

## Quick Start

**Precompiled packages**: The easiest way to install i2pd is by using precompiled packages and binaries from the [release page](https://github.com/PurpleI2P/i2pd/releases/latest). See the [official documentation](https://i2pd.readthedocs.io/en/latest/user-guide/install/) for more info.

**Building from source**: Follow the instructions below.

## Building from Source

### Prerequisites

- C++ compiler with C++17 support (g++ 7+ or clang++ 5+)
- CMake 3.8+ or Make
- OpenSSL development libraries
- Boost libraries (1.58+)
- zlib development libraries

On Debian/Ubuntu:
```bash
sudo apt-get install build-essential libssl-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-date-time-dev zlib1g-dev
```

### Build Steps

1. Clone the repository:
```bash
git clone https://github.com/PurpleI2P/i2pd.git
cd i2pd
```

2. Build i2pd:
```bash
make
```

Or with multiple cores:
```bash
make -j4
```

3. The binary will be created in the root directory as `i2pd`.

## Critical: Reseed Certificates Setup

**IMPORTANT**: i2pd requires reseed certificates to bootstrap into the I2P network. Without these certificates, i2pd will fail to connect to the I2P network, resulting in errors like:
- `Reseed: Certificate for <name> not loaded`
- `Reseed: SU3 verification failed`
- `Tunnels: Can't create outbound tunnel, no peers available`

### Installing Reseed Certificates

i2pd automatically checks **both** user and system certificate locations, so you can install certificates in either location (or both). Duplicate certificates are automatically detected and only loaded once.

**User location** (recommended for per-user installations):
```bash
# Create the certificates directory if it doesn't exist
mkdir -p ~/.i2pd/certificates/reseed

# Copy reseed certificates from the source tree
cp contrib/certificates/reseed/*.crt ~/.i2pd/certificates/reseed/
```

**System location** (for system-wide installations):
```bash
# For system-wide installation
sudo mkdir -p /var/lib/i2pd/certificates/reseed
sudo cp contrib/certificates/reseed/*.crt /var/lib/i2pd/certificates/reseed/
```

**Note**: If you have a `cp` alias configured (e.g., `alias cp='cp --reflink=auto'`), it will automatically use reflinks when supported by your filesystem, and fall back to regular copy when not supported. This is the recommended configuration.

**Alternative method** (using `install` command):
```bash
# For user location
install -m 644 contrib/certificates/reseed/*.crt ~/.i2pd/certificates/reseed/

# For system location
sudo install -m 644 contrib/certificates/reseed/*.crt /var/lib/i2pd/certificates/reseed/
```

**How it works**: i2pd checks both locations in this order:
1. User location: `~/.i2pd/certificates/reseed/` (or the directory specified by `certsdir` in your config)
2. System location: `/var/lib/i2pd/certificates/reseed/`

Certificates found in either location are loaded, and duplicates (same filename) are automatically skipped to avoid loading the same certificate twice.

### Verifying Certificates

After copying, verify the certificates are in place:
```bash
ls -la ~/.i2pd/certificates/reseed/
```

You should see multiple `.crt` files (typically 12-13 files), including:
- `igor_at_novg.net.crt`
- `creativecowpat_at_mail.i2p.crt`
- `lazygravy_at_mail.i2p.crt`
- `r4sas-reseed_at_mail.i2p.crt`
- `i2p-reseed_at_mk16.de.crt`
- And others...

### System-wide Installation

For system-wide installations, you can place certificates in the system location (`/var/lib/i2pd/certificates/reseed/`). i2pd will automatically find them there, even if `certsdir` is not explicitly set in the configuration.

If you want to use a custom certificates directory, set it in `i2pd.conf`:
```
certsdir = /var/lib/i2pd/certificates
```

**Note**: With the dual-location support, certificates in the system location are automatically checked even if `certsdir` points to a user location. This provides flexibility for mixed installations where some certificates are system-wide and others are user-specific.

## Troubleshooting

### Issue: "Reseed: Certificate for X not loaded"

This indicates missing reseed certificates. Check that certificates exist in either:
- User location: `~/.i2pd/certificates/reseed/` (or your configured `certsdir`)
- System location: `/var/lib/i2pd/certificates/reseed/`

Follow the "Installing Reseed Certificates" section above to install them in either location.

### Issue: "Tunnels: Can't create outbound tunnel, no peers available"

This usually means:
1. Reseed certificates are missing (see above)
2. Reseed failed and i2pd has no router information in `~/.i2pd/netDb/`
3. Network connectivity issues preventing reseed servers from being reached

Check your i2pd log for reseed errors:
```bash
grep -i reseed ~/.i2pd/i2pd.log
```

## Running i2pd

After building and setting up certificates:

```bash
./i2pd --conf=/etc/i2pd/i2pd.conf --tunconf=/etc/i2pd/tunnels.conf --datadir=~/.i2pd
```

Or use your system's service configuration if installed via package manager.

## Supported Systems

* GNU/Linux (Debian, Ubuntu, etc) - [![Build on Ubuntu](https://github.com/PurpleI2P/i2pd/actions/workflows/build.yml/badge.svg)](https://github.com/PurpleI2P/i2pd/actions/workflows/build.yml)
* CentOS, Fedora, Mageia - [![Build Status](https://copr.fedorainfracloud.org/coprs/supervillain/i2pd/package/i2pd-git/status_image/last_build.png)](https://copr.fedorainfracloud.org/coprs/supervillain/i2pd/package/i2pd-git/)
* Alpine, ArchLinux, openSUSE, Gentoo, etc.
* Windows - [![Build on Windows](https://github.com/PurpleI2P/i2pd/actions/workflows/build-windows.yml/badge.svg)](https://github.com/PurpleI2P/i2pd/actions/workflows/build-windows.yml)
* Mac OS - [![Build on OSX](https://github.com/PurpleI2P/i2pd/actions/workflows/build-osx.yml/badge.svg)](https://github.com/PurpleI2P/i2pd/actions/workflows/build-osx.yml)
* Docker image - [![Build containers](https://github.com/PurpleI2P/i2pd/actions/workflows/docker.yml/badge.svg)](https://github.com/PurpleI2P/i2pd/actions/workflows/docker.yml)
* Snap - [![i2pd](https://snapcraft.io/i2pd/badge.svg)](https://snapcraft.io/i2pd) [![i2pd](https://snapcraft.io/i2pd/trending.svg?name=0)](https://snapcraft.io/i2pd)
* FreeBSD - [![Build on FreeBSD](https://github.com/PurpleI2P/i2pd/actions/workflows/build-freebsd.yml/badge.svg)](https://github.com/PurpleI2P/i2pd/actions/workflows/build-freebsd.yml)
* Android - See [i2pd-android](https://github.com/PurpleI2P/i2pd-android) repository
* iOS - See [documentation](https://i2pd.readthedocs.io/en/latest/devs/building/ios/)

## Additional Resources

- [Official Documentation](https://i2pd.readthedocs.io/en/latest/)
- [Configuration Guide](https://i2pd.readthedocs.io/en/latest/user-guide/configuration/)
- [Building Documentation](https://i2pd.readthedocs.io/en/latest/devs/building/)
- [Unix Build Instructions](https://i2pd.readthedocs.io/en/latest/devs/building/unix/)
- [Windows Build Instructions](https://i2pd.readthedocs.io/en/latest/devs/building/windows/)
- [iOS Build Instructions](https://i2pd.readthedocs.io/en/latest/devs/building/ios/)
- [Android Build Instructions](https://i2pd.readthedocs.io/en/latest/devs/building/android/)
