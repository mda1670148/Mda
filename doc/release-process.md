Release Process
====================

Before every release candidate:

* Update translations (ping Fuzzbawls on Slack) see [translation_process.md](https://github.com/Aratriton-Project/Aratriton/blob/master/doc/translation_process.md#synchronising-translations).

Before every minor and major release:

* Update version in `configure.ac` (don't forget to set `CLIENT_VERSION_IS_RELEASE` to `true`)
* Write release notes (see below)

Before every major release:

* Update hardcoded [seeds](/contrib/seeds/README.md), see [this pull request](https://github.com/bitcoin/bitcoin/pull/7415) for an example.
* Update [`BLOCK_CHAIN_SIZE`](/src/qt/intro.cpp) to the current size plus some overhead.
* Update `src/chainparams.cpp` with statistics about the transaction count and rate.
* Update version of `contrib/gitian-descriptors/*.yml`: usually one'd want to do this on master after branching off the release - but be sure to at least do it before a new major release

### First time / New builders

If you're using the automated script (found in [contrib/gitian-build.sh](/contrib/gitian-build.sh)), then at this point you should run it with the "--setup" command. Otherwise ignore this.

Check out the source code in the following directory hierarchy.

    cd /path/to/your/toplevel/build
    git clone https://github.com/aratriton-project/gitian.sigs.git
    git clone https://github.com/aratriton-project/aratriton-detached-sigs.git
    git clone https://github.com/devrandom/gitian-builder.git
    git clone https://github.com/aratriton-project/aratriton.git

### Aratriton maintainers/release engineers, suggestion for writing release notes

Write release notes. git shortlog helps a lot, for example:

    git shortlog --no-merges v(current version, e.g. 0.7.2)..v(new version, e.g. 0.8.0)


Generate list of authors:

    git log --format='%aN' "$*" | sort -ui | sed -e 's/^/- /'

Tag version (or release candidate) in git

    git tag -s v(new version, e.g. 0.8.0)

### Setup and perform Gitian builds

If you're using the automated script (found in [contrib/gitian-build.sh](/contrib/gitian-build.sh)), then at this point you should run it with the "--build" command. Otherwise ignore this.

Setup Gitian descriptors:

    pushd ./aratriton
    export SIGNER=(your Gitian key, ie bluematt, sipa, etc)
    export VERSION=(new version, e.g. 0.8.0)
    git fetch
    git checkout v${VERSION}
    popd

Ensure your gitian.sigs are up-to-date if you wish to gverify your builds against other Gitian signatures.

    pushd ./gitian.sigs
    git pull
    popd

Ensure gitian-builder is up-to-date:

    pushd ./gitian-builder
    git pull
    popd

### Fetch and create inputs: (first time, or when dependency versions change)

    pushd ./gitian-builder
    mkdir -p inputs
    wget -P inputs https://bitcoincore.org/cfields/osslsigncode-Backports-to-1.7.1.patch
    wget -P inputs http://downloads.sourceforge.net/project/osslsigncode/osslsigncode/osslsigncode-1.7.1.tar.gz
    popd

Create the OS X SDK tarball, see the [OS X readme](README_osx.md) for details, and copy it into the inputs directory.

### Optional: Seed the Gitian sources cache and offline git repositories

By default, Gitian will fetch source files as needed. To cache them ahead of time:

    pushd ./gitian-builder
    make -C ../aratriton/depends download SOURCES_PATH=`pwd`/cache/common
    popd

Only missing files will be fetched, so this is safe to re-run for each build.

NOTE: Offline builds must use the --url flag to ensure Gitian fetches only from local URLs. For example:

    pushd ./gitian-builder
    ./bin/gbuild --url aratriton=/path/to/aratriton,signature=/path/to/sigs {rest of arguments}
    popd

The gbuild invocations below <b>DO NOT DO THIS</b> by default.

### Build and sign Aratriton for Linux, Windows, and OS X:

    pushd ./gitian-builder
    ./bin/gbuild --memory 3000 --commit aratriton=v${VERSION} ../aratriton/contrib/gitian-descriptors/gitian-linux.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-linux --destination ../gitian.sigs/ ../aratriton/contrib/gitian-descriptors/gitian-linux.yml
    mv build/out/aratriton-*.tar.gz build/out/src/aratriton-*.tar.gz ../

    ./bin/gbuild --memory 3000 --commit aratriton=v${VERSION} ../aratriton/contrib/gitian-descriptors/gitian-win.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-win-unsigned --destination ../gitian.sigs/ ../aratriton/contrib/gitian-descriptors/gitian-win.yml
    mv build/out/aratriton-*-win-unsigned.tar.gz inputs/aratriton-win-unsigned.tar.gz
    mv build/out/aratriton-*.zip build/out/aratriton-*.exe ../

    ./bin/gbuild --memory 3000 --commit aratriton=v${VERSION} ../aratriton/contrib/gitian-descriptors/gitian-osx.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-osx-unsigned --destination ../gitian.sigs/ ../aratriton/contrib/gitian-descriptors/gitian-osx.yml
    mv build/out/aratriton-*-osx-unsigned.tar.gz inputs/aratriton-osx-unsigned.tar.gz
    mv build/out/aratriton-*.tar.gz build/out/aratriton-*.dmg ../

    ./bin/gbuild --memory 3000 --commit aratriton=v${VERSION} ../aratriton/contrib/gitian-descriptors/gitian-aarch64.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-aarch64 --destination ../gitian.sigs/ ../aratriton/contrib/gitian-descriptors/gitian-aarch64.yml
    mv build/out/aratriton-*.tar.gz build/out/src/aratriton-*.tar.gz ../
    popd

Build output expected:

  1. source tarball (`aratriton-${VERSION}.tar.gz`)
  2. linux 32-bit and 64-bit dist tarballs (`aratriton-${VERSION}-linux[32|64].tar.gz`)
  3. windows 32-bit and 64-bit unsigned installers and dist zips (`aratriton-${VERSION}-win[32|64]-setup-unsigned.exe`, `aratriton-${VERSION}-win[32|64].zip`)
  4. OS X unsigned installer and dist tarball (`aratriton-${VERSION}-osx-unsigned.dmg`, `aratriton-${VERSION}-osx64.tar.gz`)
  5. Gitian signatures (in `gitian.sigs/${VERSION}-<linux|{win,osx}-unsigned>/(your Gitian key)/`)

### Verify other gitian builders signatures to your own. (Optional)

Add other gitian builders keys to your gpg keyring, and/or refresh keys.

    gpg --import aratriton/contrib/gitian-keys/*.pgp
    gpg --refresh-keys

Verify the signatures

    pushd ./gitian-builder
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-linux ../aratriton/contrib/gitian-descriptors/gitian-linux.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-unsigned ../aratriton/contrib/gitian-descriptors/gitian-win.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-unsigned ../aratriton/contrib/gitian-descriptors/gitian-osx.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-aarch64 ../aratriton/contrib/gitian-descriptors/gitian-aarch64.yml
    popd

### Next steps:

Commit your signature to gitian.sigs:

    pushd gitian.sigs
    git add ${VERSION}-linux/${SIGNER}
    git add ${VERSION}-win-unsigned/${SIGNER}
    git add ${VERSION}-osx-unsigned/${SIGNER}
    git add ${VERSION}-aarch64/${SIGNER}
    git commit -a
    git push  # Assuming you can push to the gitian.sigs tree
    popd

Codesigner only: Create Windows/OS X detached signatures:
- Only one person handles codesigning. Everyone else should skip to the next step.
- Only once the Windows/OS X builds each have 3 matching signatures may they be signed with their respective release keys.

Codesigner only: Sign the osx binary:

    transfer aratriton-osx-unsigned.tar.gz to osx for signing
    tar xf aratriton-osx-unsigned.tar.gz
    ./detached-sig-create.sh -s "Key ID"
    Enter the keychain password and authorize the signature
    Move signature-osx.tar.gz back to the gitian host

Codesigner only: Sign the windows binaries:

    tar xf aratriton-win-unsigned.tar.gz
    ./detached-sig-create.sh -key /path/to/codesign.key
    Enter the passphrase for the key when prompted
    signature-win.tar.gz will be created

Codesigner only: Commit the detached codesign payloads:

    cd ~/aratriton-detached-sigs
    checkout the appropriate branch for this release series
    rm -rf *
    tar xf signature-osx.tar.gz
    tar xf signature-win.tar.gz
    git add -a
    git commit -m "point to ${VERSION}"
    git tag -s v${VERSION} HEAD
    git push the current branch and new tag

Non-codesigners: wait for Windows/OS X detached signatures:

- Once the Windows/OS X builds each have 3 matching signatures, they will be signed with their respective release keys.
- Detached signatures will then be committed to the [aratriton-detached-sigs](https://github.com/Aratriton-Project/aratriton-detached-sigs) repository, which can be combined with the unsigned apps to create signed binaries.

Create (and optionally verify) the signed OS X binary:

    pushd ./gitian-builder
    ./bin/gbuild -i --commit signature=v${VERSION} ../aratriton/contrib/gitian-descriptors/gitian-osx-signer.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-osx-signed --destination ../gitian.sigs/ ../aratriton/contrib/gitian-descriptors/gitian-osx-signer.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-signed ../aratriton/contrib/gitian-descriptors/gitian-osx-signer.yml
    mv build/out/aratriton-osx-signed.dmg ../aratriton-${VERSION}-osx.dmg
    popd

Create (and optionally verify) the signed Windows binaries:

    pushd ./gitian-builder
    ./bin/gbuild -i --commit signature=v${VERSION} ../aratriton/contrib/gitian-descriptors/gitian-win-signer.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-win-signed --destination ../gitian.sigs/ ../aratriton/contrib/gitian-descriptors/gitian-win-signer.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-signed ../aratriton/contrib/gitian-descriptors/gitian-win-signer.yml
    mv build/out/aratriton-*win64-setup.exe ../aratriton-${VERSION}-win64-setup.exe
    mv build/out/aratriton-*win32-setup.exe ../aratriton-${VERSION}-win32-setup.exe
    popd

Commit your signature for the signed OS X/Windows binaries:

    pushd gitian.sigs
    git add ${VERSION}-osx-signed/${SIGNER}
    git add ${VERSION}-win-signed/${SIGNER}
    git commit -a
    git push  # Assuming you can push to the gitian.sigs tree
    popd

### After 3 or more people have gitian-built and their results match:

- Create `SHA256SUMS.asc` for the builds, and GPG-sign it:

```bash
sha256sum * > SHA256SUMS
```

The list of files should be:
```
aratriton-${VERSION}-aarch64-linux-gnu.tar.gz
aratriton-${VERSION}-arm-linux-gnueabihf.tar.gz
aratriton-${VERSION}-i686-pc-linux-gnu.tar.gz
aratriton-${VERSION}-x86_64-linux-gnu.tar.gz
aratriton-${VERSION}-osx64.tar.gz
aratriton-${VERSION}-osx.dmg
aratriton-${VERSION}.tar.gz
aratriton-${VERSION}-win32-setup.exe
aratriton-${VERSION}-win32.zip
aratriton-${VERSION}-win64-setup.exe
aratriton-${VERSION}-win64.zip
```
The `*-debug*` files generated by the gitian build contain debug symbols
for troubleshooting by developers. It is assumed that anyone that is interested
in debugging can run gitian to generate the files for themselves. To avoid
end-user confusion about which file to pick, as well as save storage
space *do not upload these to the aratriton.org server*.

- GPG-sign it, delete the unsigned file:
```
gpg --digest-algo sha256 --clearsign SHA256SUMS # outputs SHA256SUMS.asc
rm SHA256SUMS
```
(the digest algorithm is forced to sha256 to avoid confusion of the `Hash:` header that GPG adds with the SHA256 used for the files)
Note: check that SHA256SUMS itself doesn't end up in SHA256SUMS, which is a spurious/nonsensical entry.

- Upload zips and installers, as well as `SHA256SUMS.asc` from last step, to the GitHub release (see below)

- Announce the release:

  - bitcointalk announcement thread

  - Optionally twitter, reddit /r/aratriton, ... but this will usually sort out itself

  - Archive release notes for the new version to `doc/release-notes/` (branch `master` and branch of the release)

  - Create a [new GitHub release](https://github.com/Aratriton-Project/Aratriton/releases/new) with a link to the archived release notes.

  - Celebrate
