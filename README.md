# Tips For Developing With GDNative And Not Smashing Your Keyboard In Frustration (WIP)
1. `_init()` is a required function
2. Passing too few arguments to a NativeScript function will cause Godot to silently crash
3. Want to hold a Resource? use Ref<>
4. Want to use a Ref<>? Make sure to initialize the Ref<> and use Ref.instance(). Don't you dare pass Ref<> a pre-existing pointer.
5. If an internal class throws errors about unknown classes, make sure to #include everything it complains about
