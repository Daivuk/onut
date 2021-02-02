This scripts go through all bark's C++ components and generate code for
it's properties to:
- Public variable
- Hook with Javascript
- Hook with Editor UI
- Serialize
- Deserialize

So we don't have to do these manually. I did it this way because I didn't want
to generate a macro mess, or a template mess.

This is similar to what Unreal does with its UPROPERTY(...)
