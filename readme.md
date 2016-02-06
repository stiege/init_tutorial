# Linux initialisation

This repository provides a quick tutorial on the linux initialisation system. Unfortunately due to my lack of knowledge of linker scripts I used a default linker script as a template - this likely makes the project rather unportable. However as the output doesn't change the diffs can still be perused and are summarised here in the readme.

## The initial implementation

Here is the initial implementation. The initialisation functions have been
placed into an array which the main function loops through in order to
initialise.

The output is (and always will be):

```
Beginning Init
init1 called
init2 called
End of init
```
Initial implementation:

```c
#include "stdio.h"
typedef void (init_func)(void);

void init1(void)
{
    printf("%s\n", "init1 called");
}

void init2(void)
{
    printf("%s\n", "init2 called");
}

init_func * init_array[] = {
    &init1,
    &init2
};

init_func ** __init_start = &init_array[0];
init_func ** __init_end = &init_array[1];

void main(void)
{
    printf("%s\n", "Beginning Init");
    init_func ** inits = __init_start;

    if (NULL != __init_start)
    {
        do
        {
            (*inits)();
            inits++;
        }while (inits <= __init_end);
    }
    printf("%s\n", "End of init");
}
```

## Second commit - moving functions to separate source

Not much happens in the second commit - the init functions are moved to another
source file.

```diff
diff --git a/src/empty b/src/empty
deleted file mode 100755
index c447401..0000000
Binary files a/src/empty and /dev/null differ
diff --git a/src/init_defs.h b/src/init_defs.h
new file mode 100644
index 0000000..5bb97a2
--- /dev/null
+++ b/src/init_defs.h
@@ -0,0 +1 @@
+typedef void (init_func)(void);
diff --git a/src/main.c b/src/main.c
index 2141d2e..f431db4 100644
--- a/src/main.c
+++ b/src/main.c
@@ -1,36 +1,24 @@
 #include "stdio.h"
-typedef void (init_func)(void);
+#include "init_defs.h"
 
-void init1(void)
-{
-    printf("%s\n", "init1 called");
-}
-
-void init2(void)
-{
-    printf("%s\n", "init2 called");
-}
-
-init_func * init_array[] = {
-    &init1,
-    &init2
-};
-
-init_func ** __init_start = &init_array[0];
-init_func ** __init_end = &init_array[1];
+extern init_func ** __init_start;
+extern init_func ** __init_end;
 
 void main(void)
 {
-    printf("%s\n", "Beginning Init");
     init_func ** inits = __init_start;
 
     if (NULL != __init_start)
     {
+        printf("%s\n", "Beginning Init");
         do
         {
-            (*inits)();
-            inits++;
+            (*inits++)();
         }while (inits <= __init_end);
+        printf("%s\n", "End of init");
+    }
+    else
+    {
+        printf("%s\n", "No init functions registered");
     }
-    printf("%s\n", "End of init");
 }
diff --git a/src/main.cmake b/src/main.cmake
index 00e7e60..958f200 100644
--- a/src/main.cmake
+++ b/src/main.cmake
@@ -1,6 +1,6 @@
 
 set(LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/linker.ld")
 set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")
-# set(CMAKE_EXE_LINKER_FLAGS "-nostdlib")
-# set(CMAKE_C_FLAGS "-static")
-add_executable(out "${CMAKE_CURRENT_LIST_DIR}/main.c")
+add_executable(out
+    "${CMAKE_CURRENT_LIST_DIR}/main.c"
+    "${CMAKE_CURRENT_LIST_DIR}/requires_init.c")
diff --git a/src/requires_init.c b/src/requires_init.c
new file mode 100644
index 0000000..03eaed5
--- /dev/null
+++ b/src/requires_init.c
@@ -0,0 +1,20 @@
+#include "stdio.h"
+#include "init_defs.h"
+
+void init1(void)
+{
+    printf("%s\n", "init1 called");
+}
+
+void init2(void)
+{
+    printf("%s\n", "init2 called");
+}
+
+init_func * init_array[] = {
+    &init1,
+    &init2
+};
+
+init_func ** __init_start = &init_array[0];
+init_func ** __init_end = &init_array[1];
```

### Commit 3 - linker script modification

In commit 3 the mechanics of the linux init system start to come forward. A
section is declared in the linker script of ".init_functions". and
`__init_start` and `__init_end` are used to point to the memory locations at and
after this section. The main function now uses these pointers and the
initialisation array is registered to this location by use of
`__attribute__((section(".init_functions")))`.

```diff
diff --git a/src/linker.ld b/src/linker.ld
index 4a6a7c7..39df084 100644
--- a/src/linker.ld
+++ b/src/linker.ld
@@ -65,6 +65,12 @@ SECTIONS
   {
     KEEP (*(SORT_NONE(.fini)))
   }
+  .init_functions :
+  {
+    PROVIDE (__init_start = .);
+    *(.init_functions)
+    PROVIDE (__init_end = .);
+  }
   PROVIDE (__etext = .);
   PROVIDE (_etext = .);
   PROVIDE (etext = .);
diff --git a/src/main.c b/src/main.c
index f431db4..3e8b1a4 100644
--- a/src/main.c
+++ b/src/main.c
@@ -1,20 +1,20 @@
 #include "stdio.h"
 #include "init_defs.h"
 
-extern init_func ** __init_start;
-extern init_func ** __init_end;
+extern init_func * __init_start;
+extern init_func * __init_end;
 
 void main(void)
 {
-    init_func ** inits = __init_start;
+    init_func ** inits = &__init_start;
 
-    if (NULL != __init_start)
+    if (__init_start != __init_end)
     {
         printf("%s\n", "Beginning Init");
         do
         {
-            (*inits++)();
-        }while (inits <= __init_end);
+            (**inits++)();
+        } while (inits < &__init_end);
         printf("%s\n", "End of init");
     }
     else
diff --git a/src/requires_init.c b/src/requires_init.c
index 03eaed5..d98c4d9 100644
--- a/src/requires_init.c
+++ b/src/requires_init.c
@@ -6,15 +6,13 @@ void init1(void)
     printf("%s\n", "init1 called");
 }
 
-void init2(void)
+void init2 (void)
 {
     printf("%s\n", "init2 called");
 }
 
+__attribute__((section(".init_functions")))
 init_func * init_array[] = {
     &init1,
     &init2
 };
-
-init_func ** __init_start = &init_array[0];
-init_func ** __init_end = &init_array[1];
```

### Commit 4 - macro definition

The final step is simply to provide a macro so that individual init functions
can be registered rather than using an array which has to know about every
initialisation function in the project. Now the init functions can even be
made `static`.

```diff
diff --git a/src/init_defs.h b/src/init_defs.h
index 5bb97a2..8a6f56f 100644
--- a/src/init_defs.h
+++ b/src/init_defs.h
@@ -1 +1,5 @@
 typedef void (init_func)(void);
+
+#define REGISTER_INIT_FUNCTION(func)        \
+__attribute__((section(".init_functions"))) \
+init_func * func ## _registration_ptr = &func;
diff --git a/src/requires_init.c b/src/requires_init.c
index d98c4d9..2a4dfeb 100644
--- a/src/requires_init.c
+++ b/src/requires_init.c
@@ -1,18 +1,15 @@
 #include "stdio.h"
 #include "init_defs.h"
 
-void init1(void)
+static void init1(void)
 {
     printf("%s\n", "init1 called");
 }
 
-void init2 (void)
+static void init2 (void)
 {
     printf("%s\n", "init2 called");
 }
 
-__attribute__((section(".init_functions")))
-init_func * init_array[] = {
-    &init1,
-    &init2
-};
+REGISTER_INIT_FUNCTION(init1);
+REGISTER_INIT_FUNCTION(init2);
```