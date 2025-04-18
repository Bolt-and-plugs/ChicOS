# Logging macros

Use these macros instead of calling c_log() directly:

```c
c_debug(<status_code>, "Debug message");  
c_info("Informational message");
c_warn(<status_code>, "Error", reason);
c_error(<status_code>, "Error", errnum);
c_crit_error(<status_code>, "Fatal error!");
```
