
U8 videoMem[256]; 

static F16 Lerp(F16 a, F16 b, F16 t)
{
    return a + (b - a) * t;
}

U0 Main()
{
    U8* message = "Hello, World";
    Print(message);
}