"# yarm" 

Code developed alongside Mikko Oijala.
Used to run a live state machine for a novel freely-moving rodent behavior.
I switched from our head-fixed setup in favor of a freely-moving paradigm in order to accelerate mice learning rates.
I chose a y-maze design over a t-maze so that the next trial can begin where the last one left off, without the animal
needing to return to a specific arm. This allowed me to double my trial throughput. Most y-maze decision-making paradigms
use localized stimuli responses. I ended up using this a shaping step (which I refer to as the Follow the Pattern stage).
However, ultimately the mice must learn the meaning of the stimuli, where horizontal gratings tell the mouse to turn left
and vertical gratings to turn right. In this final stage (Initiation Arm Only), there is only a single centralized visual
stimulus presented on a table underneath the animal. Therefore, the animal most perform true decision-making.

![fig11_yarm_rig_freelymoving](https://github.com/aozgur360/yarm/assets/77759136/777ec607-d1a5-4539-8efe-a5117c6d7d7d)
![fig2_stimuli_freelymoving](https://github.com/aozgur360/yarm/assets/77759136/b21f38f9-b3aa-48c0-bbe5-dde92bec7895)
![fig3_shapingsteps_freelymoving](https://github.com/aozgur360/yarm/assets/77759136/4d0e7e20-6967-4846-8608-7e16f02d61c0)
![fig16_yarm_gui_freelymoving](https://github.com/aozgur360/yarm/assets/77759136/eab88eb1-a7f5-4282-8f46-7437d10f80b6)
![fig17_learingrates_FTP_INIT](https://github.com/aozgur360/yarm/assets/77759136/f76942bf-6dfc-4241-a434-7aacc2f4b237)
![fig18_mice_memory_yarm_INIT](https://github.com/aozgur360/yarm/assets/77759136/d9f10470-7ddd-44bc-aa84-0611843a1aaf)
![fig19_surgery_timeline](https://github.com/aozgur360/yarm/assets/77759136/6ae655a6-88d4-4218-8056-6a36bf574e7d)
![fig20_baseplating](https://github.com/aozgur360/yarm/assets/77759136/9e9bab22-70ae-41dd-bdd4-4ee157520eae)
![fig22_muscimol](https://github.com/aozgur360/yarm/assets/77759136/3837da5b-7b53-481a-b73e-2a1fc283930c)
![fig21_example_catraces](https://github.com/aozgur360/yarm/assets/77759136/f4e3579f-dfe8-45f0-8760-b58615f4b1a6)

While this project focused on establishing the physical behavior, please refer to my repository titled "yarm-neural-analysis"
for analysis on the neural activity during the behavior. This includes decoding with neural activity, clustering by thousands of
neurons across mice to analyze learning, and assessing ensemble stability.




