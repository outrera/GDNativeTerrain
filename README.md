# Tips For Developing With GDNative And Not Smashing Your Keyboard In Frustration (WIP)
1. `_init()` is a required function
2. Passing too few arguments to a NativeScript function will cause Godot to silently crash
3. Want to hold a Resource? use Ref<>
4. If an internal class throws errors about unknown classes, make sure to #include everything it complains about
