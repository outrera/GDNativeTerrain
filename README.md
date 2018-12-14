# Tips For Developing With GDNative And Not Smashing Your Keyboard In Frustration
1. `_init()` is a required function
2. Passing too few arguments through GDScript to a NativeScript method will cause Godot to silently crash
3. `_register_methods()` is where you put your methods that you want to expose to GDScript
