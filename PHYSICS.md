# Physics Engine — Documentation

## Constants

| Constant | Value | Unit | Purpose |
|---|---|---|---|
| `GRAVITY` | `9.81` | pixels/s² | Downward acceleration applied to all particles each step |
| `RESTITUTION` | `0.8` | dimensionless (0–1) | Energy retained after a bounce. `1.0` = perfectly elastic, `0.0` = no bounce |
| `DT` | `1/60 ≈ 0.01667` | seconds | Fixed simulation time step — one physics update per frame |
| `WINDOW_WIDTH` | `1000` | pixels | Horizontal extent of the simulation space |
| `WINDOW_HEIGHT` | `800` | pixels | Vertical extent / floor boundary |

---

## Coordinate System

The y-axis points **downward** (standard screen space). This means:

- `y = 0` is the top of the window
- `y = WINDOW_HEIGHT` is the bottom (the floor)
- Gravity adds to `velocity.y`, so the ball falls toward higher y values

---

## Data Structures

### `vector2`
A simple 2D container holding `x` and `y` as floats. Used for both position and velocity.

### `Particle`
Represents a physical object in the simulation.

| Field | Type | Description |
|---|---|---|
| `position` | `vector2` | Current location in pixels |
| `velocity` | `vector2` | Current speed in pixels/second |
| `mass` | `float` | Mass in arbitrary units (unused in current forces, reserved for future use) |
| `radius` | `float` | Visual and collision radius in pixels |

---

## Integration Method — Semi-Implicit Euler

Each simulation step updates the particle in this order:

```
velocity += gravity * dt       // 1. update velocity using current forces
position += velocity * dt      // 2. update position using the NEW velocity
```

This is called **semi-implicit (symplectic) Euler** integration. The key distinction from plain explicit Euler is step order — position uses the already-updated velocity, not the old one.

### Why this order matters

| Method | Position uses | Energy behavior |
|---|---|---|
| Explicit Euler | old velocity | Gains energy over time (unstable) |
| Semi-implicit Euler | new velocity | Conserves energy better (stable) |

---

## Kinematic Equations

After **N** simulation steps from rest, the exact values the simulation produces are:

**Velocity:**
```
v(N) = N × GRAVITY × DT
```

**Cumulative displacement** (semi-implicit Euler sum):
```
Δy(N) = GRAVITY × DT² × N(N+1)/2
```

This is the discrete equivalent of the continuous kinematic formula `Δy = ½gt²`.

### Example — after 61 steps (~1 simulated second)

```
v  = 61 × 9.81 × (1/60) ≈ 9.9735 pixels/s
Δy = 9.81 × (1/60)² × (61×62/2) ≈ 5.153 pixels
```

These match the first logged output values.

---

## Logging

The simulation logs position and velocity approximately once per simulated second using a `logTimer` accumulator. It only logs while the ball is still within the window boundary:

```
condition: logTimer >= 1.0  AND  (ball.y + ball.radius) <= WINDOW_HEIGHT
```

The log fires after ~61 steps (not 60) due to floating-point representation of `1.0f/60.0f`, which accumulates slightly above `1.0` at 61 multiples before resetting.

---

## Future Considerations

- `RESTITUTION` is defined but not yet applied — it will be used to scale `velocity.y` on floor collision: `velocity.y = -velocity.y * RESTITUTION`
- `mass` is defined but currently unused — needed for force-based simulation (`F = ma`) and collision response
- No frame-rate limiter is set; the physics step count per real second is uncapped
