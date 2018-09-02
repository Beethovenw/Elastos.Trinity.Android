# Elastos.Trinity.Android
# submit google chromium code to github
- firstly delete each third party's .git repository
- delete .gitignore
- pull chromium code from google and submite to git@github.com:elastos/Elastos.Trinity.Android.git
- remerge .gitignore file deleted before （major reason is .gitignore ignore the code category of dependent third party libary，and it produced many files, and we do not have to submit）

# compile code from github

```
git clone https://github.com/elastos/Elastos.Trinity.Android.git elastos
cd elastos/src
export PATH="your_dir/elastos/depot_tools$PATH"
gn args out/chrome
```
gn configurated parameters：
```
target_os="android"
target_cpu = "arm"
is_debug=false
symbol_level=1
is_official_build=true
is_chrome_branded=false
use_official_google_api_keys=false
enable_resource_whitelist_generation=false
ffmpeg_branding="Chrome"
proprietary_codecs=true
enable_remoting=true
```

start to compile
```
ninja -C out/chrome chrome_public_apk
```

git clone often failed because of lots of code,please use git fetch to download the code:

```
git init
git fetch https://github.com/elastos/Elastos.Trinity.Android.git
git checkout FETCH_HEAD
```

61e373a677a42fcdc951221ba2d0d16d13c738a8
