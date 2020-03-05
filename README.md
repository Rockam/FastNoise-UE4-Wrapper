# FastNoise UE4 Wrapper

Unreal Engine 4 intuitive, open source and blueprint-friendly noise generation library coded on C++. It uses [Auburns's FastNoise library](https://github.com/Auburns/FastNoise).

**I've released a free plugin on the UE4 marketplace called [Fast Noise Generator (FNG)](https://www.unrealengine.com/marketplace/en-US/product/527737f42da544e9abf8607a40495fbd), check it out!!!**

## Usage

The most important part when compiling the wrapper into your UE4 project is replacing **PROJECT_API** on **FastNoiseWrapper.h** with your project's one.

### Blueprints

The wrapper offers blueprint nodes for complete usage. The first step would be to create a variable of **FastNoiseWrapper** type, then setup it and finally getting a 2D/3D value of noise. Blueprint nodes can be seen below: 

![FastNoise wrapper blueprint nodes](https://github.com/Rockam/FastNoise-UE4-Wrapper/blob/master/Blueprint_nodes.JPG "FastNoise wrapper blueprint nodes")

### C++ 

First you have to create a pointer to a **UFastNoiseWrapper** object, then setup it and finally getting a 2D/3D value of noise using the provided functions. Example code is provided below:

```cpp
UFastNoiseWrapper* fastNoiseWrapper = nullptr;
float noise2D = 0.0f;
float noise3D = 0.0f;

fastNoiseWrapper = CreateDefaultSubobject<UFastNoiseWrapper>(TEXT("FastNoiseWrapper"));

fastNoiseWrapper->SetupFastNoise(
		NoiseType, Seed, Frequency, Interpolation,
		FractalType, FractalOctaves, FractalLacunarity, FractalGain,
		CellularJitter, CellularDistanceFunction, CellularReturnType);

noise2D = fastNoiseWrapper->GetNoise2D(x, y);
noise3D = fastNoiseWrapper->GetNoise2D(x, y, z);
```
