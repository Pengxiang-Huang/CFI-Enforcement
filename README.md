# CFI
CFI Enforecement project

## Running Guide
first to build a customized llvm version by 
```
./build-llvm.sh
```

then enable it by 
```
source enable
```

and run to build the pass 
```
./run.sh
```

then use the executable to analyzing the modules 
```
./build/src/cfi-analyzer .bc (files)
```
