// FastNoiseWrapper.h
//
// MIT License
//
// Copyright(c) 2019 Víctor Hernández
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is hm.victor.92@gmail.com
//

// VERSION: 1.0.0

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FastNoise.h"
#include "FastNoiseWrapper.generated.h"

// Fast Noise UE4 enum wrappers
UENUM(BlueprintType) enum class EFastNoise_NoiseType				: uint8 { Value, ValueFractal, Perlin, PerlinFractal, Simplex, SimplexFractal, Cellular, WhiteNoise, Cubic, CubicFractal };
UENUM(BlueprintType) enum class EFastNoise_Interp					: uint8 { Linear, Hermite, Quintic };
UENUM(BlueprintType) enum class EFastNoise_FractalType				: uint8 { FBM, Billow, RigidMulti };
UENUM(BlueprintType) enum class EFastNoise_CellularDistanceFunction	: uint8 { Euclidean, Manhattan, Natural };
UENUM(BlueprintType) enum class EFastNoise_CellularReturnType		: uint8 { CellValue, /*NoiseLookup,*/ Distance, Distance2, Distance2Add, Distance2Sub, Distance2Mul, Distance2Div };

/**
 * UE4 Wrapper for Auburns's FastNoise library, also available for blueprints usage
 */
UCLASS(BlueprintType)
class PROCEDURALGENERATION_API UFastNoiseWrapper : public UObject
{
	GENERATED_BODY()

public:

	/**
	* Set all the properties needed to generate the noise
	*
	* @param noiseType					- noise return type of GetNoise(...). Default value: Simplex
	* @param seed						- seed used for all noise types. Using different seeds will cause the noise output to change. Default value: 1337
	* @param frequency					- frequency for all noise types, except White Noise. Affects how coarse the noise output is. Default value: 0.01
	* @param interp						- interpolation method used to smooth between noise values in Value and Perlin Noise. Possible interpolation methods (lowest to highest quality): Linear; Hermite; Quintic. Default value: Quintic
	* @param fractaltype				- method for combining octaves in all fractal noise types. Default value: FBM
	* @param octaves					- octave count for all fractal noise types. The amount of noise layers used to create the fractal. Default value: 3
	* @param lacunarity					- octave lacunarity for all fractal noise types. The frequency multiplier between each octave. Default value: 2.0
	* @param gain						- octave gain for all fractal noise types. The relative strength of noise from each layer when compared to the last. Default value: 0.5
	* @param cellularJitter				- maximum distance a cellular point can move from its grid position. Setting this high will make artifacts more common. Default value: 0.45
	* @param cellularDistanceFunction	- distance function used in cellular noise calculations. The distance function used to calculate the cell for a given point. Natural is a blend of Euclidean and Manhattan to give curved cell boundaries. Default value: Euclidean
	* @param cellularReturnType			- return type from cellular noise calculations. Default value: CellValue
	*/
	UFUNCTION(BlueprintCallable, Category = "Fast Noise")
	void SetupFastNoise
	(
		const EFastNoise_NoiseType noiseType = EFastNoise_NoiseType::Simplex,
		const int32 seed = 1337,
		const float frequency = 0.01f,
		const EFastNoise_Interp interp = EFastNoise_Interp::Quintic,
		const EFastNoise_FractalType fractaltype = EFastNoise_FractalType::FBM,
		const int32 octaves = 3,
		const float lacunarity = 2.0f,
		const float gain = 0.5f,
		const float cellularJitter = 0.45f,
		const EFastNoise_CellularDistanceFunction cellularDistanceFunction = EFastNoise_CellularDistanceFunction::Euclidean,
		const EFastNoise_CellularReturnType cellularReturnType = EFastNoise_CellularReturnType::CellValue
	)
	{
		SetNoiseType(noiseType);
		SetSeed(seed);
		SetFrequency(frequency);
		SetInterpolation(interp);
		SetFractalType(fractaltype);
		SetOctaves(octaves);
		SetLacunarity(lacunarity);
		SetGain(gain);
		SetCellularJitter(cellularJitter);
		SetDistanceFunction(cellularDistanceFunction);
		SetReturnType(cellularReturnType);

		bInitialized = true;
	}

	/** Returns if Fast Noise properties are initialized or not */
	UFUNCTION(BlueprintPure, Category = "Fast Noise")
	bool IsInitialized() { return bInitialized; }

	/**
	* Returns the noise calculation given x and y values
	*
	* @param x	- the x value
	* @param y	- the y value
	*/
	UFUNCTION(BlueprintCallable, Category = "Fast Noise")
	float GetNoise2D(const float x, const float y) { return IsInitialized() ? fastNoise.GetNoise(x, y) : 0.0f; }

	/**
	* Returns the noise calculation given x, y and z values
	*
	* @param x	- the x value
	* @param y	- the y value
	* @param z	- the z value
	*/
	UFUNCTION(BlueprintCallable, Category = "Fast Noise")
	float GetNoise3D(const float x, const float y, const float z = 0.0f) { return IsInitialized() ? fastNoise.GetNoise(x, y, z) : 0.0f; }


	//***********************************************************
	//*********************     GETTERS     *********************
	//***********************************************************

	/** Gets noise type */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|General settings")
	EFastNoise_NoiseType GetNoiseType()
	{
		switch (fastNoise.GetNoiseType())
		{
		case FastNoise::NoiseType::Value:
			return EFastNoise_NoiseType::Value;

		case FastNoise::NoiseType::ValueFractal:
			return EFastNoise_NoiseType::ValueFractal;

		case FastNoise::NoiseType::Perlin:
			return EFastNoise_NoiseType::Perlin;

		case FastNoise::NoiseType::PerlinFractal:
			return EFastNoise_NoiseType::PerlinFractal;

		case FastNoise::NoiseType::SimplexFractal:
			return EFastNoise_NoiseType::SimplexFractal;

		case FastNoise::NoiseType::Cellular:
			return EFastNoise_NoiseType::Cellular;

		case FastNoise::NoiseType::WhiteNoise:
			return EFastNoise_NoiseType::WhiteNoise;

		case FastNoise::NoiseType::Cubic:
			return EFastNoise_NoiseType::Cubic;

		case FastNoise::NoiseType::CubicFractal:
			return EFastNoise_NoiseType::CubicFractal;

		case FastNoise::NoiseType::Simplex:
		default:
			return EFastNoise_NoiseType::Simplex;
		}
	}

	/** Gets seed. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|General settings")
	int32 GetSeed() { return fastNoise.GetSeed(); }

	/** Gets frequency. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|General settings")
	float GetFrequency() { return fastNoise.GetFrequency(); }

	/** Gets interpolation type. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|General settings")
	EFastNoise_Interp GetInterpolation()
	{
		switch (fastNoise.GetInterp())
		{
		case FastNoise::Interp::Linear:
			return EFastNoise_Interp::Linear;

		case FastNoise::Interp::Hermite:
			return EFastNoise_Interp::Hermite;

		case FastNoise::Interp::Quintic:
		default:
			return EFastNoise_Interp::Quintic;
		}
	}

	/** Gets fractal type. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|Fractal settings")
	EFastNoise_FractalType GetFractalType()
	{
		switch (fastNoise.GetFractalType())
		{
		case FastNoise::FractalType::Billow:
			return EFastNoise_FractalType::Billow;

		case FastNoise::FractalType::RigidMulti:
			return EFastNoise_FractalType::RigidMulti;

		case FastNoise::FractalType::FBM:
		default:
			return EFastNoise_FractalType::FBM;
		}
	}

	/** Gets fractal octaves. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|Fractal settings")
	int32 GetOctaves() { return fastNoise.GetFractalOctaves(); }

	/** Gets fractal lacunarity. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|Fractal settings")
	float GetLacunarity() { return fastNoise.GetFractalLacunarity(); }

	/** Gets fractal gain. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|Fractal settings")
	float GetGain() { return fastNoise.GetFractalGain(); }

	/** Gets cellular jitter. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|Cellular settings")
	float GetCellularJitter() { return fastNoise.GetCellularJitter(); }

	/** Gets cellular distance function. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|Cellular settings")
	EFastNoise_CellularDistanceFunction GetDistanceFunction()
	{
		switch (fastNoise.GetCellularDistanceFunction())
		{
		case FastNoise::CellularDistanceFunction::Manhattan:
			return EFastNoise_CellularDistanceFunction::Manhattan;

		case FastNoise::CellularDistanceFunction::Natural:
			return EFastNoise_CellularDistanceFunction::Natural;

		case FastNoise::CellularDistanceFunction::Euclidean:
		default:
			return EFastNoise_CellularDistanceFunction::Euclidean;
		}
	}

	/** Gets cellular return type. */
	UFUNCTION(BlueprintPure, Category = "Fast Noise|Cellular settings")
	EFastNoise_CellularReturnType GetReturnType()
	{
		switch (fastNoise.GetCellularReturnType())
		{
		/*case FastNoise::CellularReturnType::NoiseLookup:
			return EFastNoise_CellularReturnType::NoiseLookup;*/

		case FastNoise::CellularReturnType::Distance:
			return EFastNoise_CellularReturnType::Distance;

		case FastNoise::CellularReturnType::Distance2:
			return EFastNoise_CellularReturnType::Distance2;

		case FastNoise::CellularReturnType::Distance2Add:
			return EFastNoise_CellularReturnType::Distance2Add;

		case FastNoise::CellularReturnType::Distance2Sub:
			return EFastNoise_CellularReturnType::Distance2Sub;

		case FastNoise::CellularReturnType::Distance2Mul:
			return EFastNoise_CellularReturnType::Distance2Mul;

		case FastNoise::CellularReturnType::Distance2Div:
			return EFastNoise_CellularReturnType::Distance2Div;

		case FastNoise::CellularReturnType::CellValue:
		default:
			return EFastNoise_CellularReturnType::CellValue;
		}
	}


	//***********************************************************
	//*********************     SETTERS     *********************
	//***********************************************************

	/** Set noise type. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|General settings")
	void SetNoiseType(const EFastNoise_NoiseType noiseType)
	{
		switch (noiseType)
		{
		case EFastNoise_NoiseType::Value:
			fastNoise.SetNoiseType(FastNoise::NoiseType::Value);
			break;
		case EFastNoise_NoiseType::ValueFractal:
			fastNoise.SetNoiseType(FastNoise::NoiseType::ValueFractal);
			break;
		case EFastNoise_NoiseType::Perlin:
			fastNoise.SetNoiseType(FastNoise::NoiseType::Perlin);
			break;
		case EFastNoise_NoiseType::PerlinFractal:
			fastNoise.SetNoiseType(FastNoise::NoiseType::PerlinFractal);
			break;
		case EFastNoise_NoiseType::SimplexFractal:
			fastNoise.SetNoiseType(FastNoise::NoiseType::SimplexFractal);
			break;
		case EFastNoise_NoiseType::Cellular:
			fastNoise.SetNoiseType(FastNoise::NoiseType::Cellular);
			break;
		case EFastNoise_NoiseType::WhiteNoise:
			fastNoise.SetNoiseType(FastNoise::NoiseType::WhiteNoise);
			break;
		case EFastNoise_NoiseType::Cubic:
			fastNoise.SetNoiseType(FastNoise::NoiseType::Cubic);
			break;
		case EFastNoise_NoiseType::CubicFractal:
			fastNoise.SetNoiseType(FastNoise::NoiseType::CubicFractal);
			break;
		case EFastNoise_NoiseType::Simplex:
		default:
			fastNoise.SetNoiseType(FastNoise::NoiseType::Simplex);
		}
	}

	/** Set seed. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|General settings")
	void SetSeed(const int32 seed) { fastNoise.SetSeed(seed); }

	/** Set frequency. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|General settings")
	void SetFrequency(const float frequency) { fastNoise.SetFrequency(frequency); }

	/** Set interpolation type. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|General settings")
	void SetInterpolation(const EFastNoise_Interp interp)
	{
		switch (interp)
		{
		case EFastNoise_Interp::Linear:
			fastNoise.SetInterp(FastNoise::Interp::Linear);
			break;
		case EFastNoise_Interp::Hermite:
			fastNoise.SetInterp(FastNoise::Interp::Hermite);
			break;
		case EFastNoise_Interp::Quintic:
		default:
			fastNoise.SetInterp(FastNoise::Interp::Quintic);
		}
	}

	/** Set fractal type. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|Fractal settings")
	void SetFractalType(const EFastNoise_FractalType fractalType)
	{ 
		switch (fractalType)
		{
		case EFastNoise_FractalType::Billow:
			fastNoise.SetFractalType(FastNoise::FractalType::Billow);
			break;
		case EFastNoise_FractalType::RigidMulti:
			fastNoise.SetFractalType(FastNoise::FractalType::RigidMulti);
			break;
		case EFastNoise_FractalType::FBM:
		default:
			fastNoise.SetFractalType(FastNoise::FractalType::FBM);
		}
	}

	/** Set fractal octaves. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|Fractal settings")
	void SetOctaves(const int32 octaves) { fastNoise.SetFractalOctaves(octaves); }

	/** Set fractal lacunarity. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|Fractal settings")
	void SetLacunarity(const float lacunarity) { fastNoise.SetFractalLacunarity(lacunarity); }

	/** Set fractal gain. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|Fractal settings")
	void SetGain(const float gain) { fastNoise.SetFractalGain(gain); }

	/** Set cellular jitter. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|Cellular settings")
	void SetCellularJitter(const float cellularJitter) { fastNoise.SetCellularJitter(cellularJitter); }

	/** Set cellular distance function. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|Cellular settings")
	void SetDistanceFunction(const EFastNoise_CellularDistanceFunction distanceFunction)
	{
		switch (distanceFunction)
		{
		case EFastNoise_CellularDistanceFunction::Manhattan:
			fastNoise.SetCellularDistanceFunction(FastNoise::CellularDistanceFunction::Manhattan);
			break;
		case EFastNoise_CellularDistanceFunction::Natural:
			fastNoise.SetCellularDistanceFunction(FastNoise::CellularDistanceFunction::Natural);
			break;
		case EFastNoise_CellularDistanceFunction::Euclidean:
		default:
			fastNoise.SetCellularDistanceFunction(FastNoise::CellularDistanceFunction::Euclidean);
		}
	}

	/** Set cellular return type. */
	UFUNCTION(BlueprintCallable, Category = "Fast Noise|Cellular settings")
	void SetReturnType(const EFastNoise_CellularReturnType cellularReturnType)
	{
		switch (cellularReturnType)
		{
		/*case EFastNoise_CellularReturnType::NoiseLookup:
			fastNoise.SetCellularReturnType(FastNoise::CellularReturnType::NoiseLookup);
			break;*/
		case EFastNoise_CellularReturnType::Distance:
			fastNoise.SetCellularReturnType(FastNoise::CellularReturnType::Distance);
			break;
		case EFastNoise_CellularReturnType::Distance2:
			fastNoise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2);
			break;
		case EFastNoise_CellularReturnType::Distance2Add:
			fastNoise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Add);
			break;
		case EFastNoise_CellularReturnType::Distance2Sub:
			fastNoise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Sub);
			break;
		case EFastNoise_CellularReturnType::Distance2Mul:
			fastNoise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Mul);
			break;
		case EFastNoise_CellularReturnType::Distance2Div:
			fastNoise.SetCellularReturnType(FastNoise::CellularReturnType::Distance2Div);
			break;
		case EFastNoise_CellularReturnType::CellValue:
		default:
			fastNoise.SetCellularReturnType(FastNoise::CellularReturnType::CellValue);
		}
	}

private:

	FastNoise fastNoise;
	bool bInitialized = false;
};
