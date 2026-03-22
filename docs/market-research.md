# ZERO — Market Research v2 (Deep Dive)
> Дата: 2026-03-22 | Парсинг: Reddit API (r/Meshtastic, r/Survival, r/backpacking, r/Garmin, r/preppers), форумы
> Метод: прямой API-парсинг постов + топ-комментарии

---

## КРИТИЧЕСКИЕ ИНСАЙТЫ (tldr)

1. **LoRa mesh = рабочая технология** — реальные люди спасали собак, координировали семьи на круизах, защищали заповедники
2. **Garmin убивают подписки** — r/Garmin бунтует: 11895 апвоутов на "DO NOT sign up for Connect+"
3. **Носимого браслета с mesh нет нигде** — только коробки и стационарные ноды
4. **Ключевой use case для охотников/туристов** — потеря члена группы (собака, ребёнок, партнёр)
5. **Wildlife/B2B рынок** — заповедники уже внедряют LoRa на 200 кв.миль для рейнджеров

---

## 1. РЕАЛЬНЫЕ USE CASES (с доказательствами)

### Use Case 1: "Meshtastic нашёл мою собаку"
**Источник:** r/Meshtastic, 451 очков, 2025
**Что произошло:** Хайкер потерял собаку в лесу. Через Meshtastic T1000-E на ошейнике (beacon каждые 15 сек) нашёл собаку в 200 метрах за густым сосновым лесом и рельефом.

> "Having this precise location saved us a lot of time and effort. Instead of blindly searching through long grass and woodland in every direction, we had a clear idea of where to focus."

**Вывод для ZERO:** GPS трекинг членов группы (и питомцев) в режиме реального времени — убийственный use case для v1.

---

### Use Case 2: "Meshtastic на семейном круизе"
**Источник:** r/Meshtastic, 1969 очков — самый популярный пост за год
**Что произошло:** Семья из 4 человек сэкономила $864 (по $27/чел/день за корабельный WiFi) используя Meshtastic с нодами WisMesh Tag на лантах и центральным нодом WisMesh Pocket. Работало внутри металлического корпуса корабля.

> "Saved a 'boatload' of money and my wife keeps praising me for how well it worked!"

**Вывод для ZERO:** Семейный сценарий (горы, поход, поездка) = самая массовая аудитория. Не нишевый продукт.

---

### Use Case 3: Wildlife Reserve South Africa — 200 кв.миль
**Источник:** r/Meshtastic, Dinoken Wildlife Reserve, 663 очков
**Что произошло:** 40 устройств FenceRanger + LoRa сеть на 160 км периметра. Система детектирует вторжение браконьеров и передаёт точное местоположение рейнджерам за 3 сек. Связана с дронами для перехвата.

> "truly transforming the patrol mode from 'post-event response' to 'real-time prevention'"

**Вывод для ZERO:** B2B рынок — охотхозяйства, заповедники, егеря. В РФ это тысячи охотничьих хозяйств площадью 1000+ га.

---

### Use Case 4: Ирак — альтернативная сеть при отключении интернета
**Источник:** r/Meshtastic, 1209 очков
**Что произошло:** Иракский энтузиаст строит mesh-сеть от южных болот до северных гор как альтернативу при правительственных отключениях.

**Вывод для ZERO:** РФ-аудитория с теми же мотивами существует — особенно после 2022. "Когда у всех ноль — у тебя всё работает."

---

### Use Case 5: La Réunion Island — Hurricane emergency network
**Источник:** r/Meshtastic, реальный кейс 2025
**Что произошло:** Сообщество из 100+ участников, 30 нод развёрнуто для экстренной связи при ураганах. Рекорд дальности: 14.2 км наземного соединения. Самолёт-нода над островом.

**Вывод для ZERO:** ZERO Node (стационарный ретранслятор) = отдельный B2C/B2G продукт.

---

## 2. БОЛИ АУДИТОРИИ (прямые цитаты)

### Боль #1: Garmin убивают подписками
**Источник:** r/Garmin, 11895 очков — топ пост за год

> "DO NOT sign up for Garmin Connect+. Unite to fight for our customers' right to software as a product"

**Пост [1138 очков]:**
> "I've been a loyal Garmin user for years... But now? They're pushing a damn subscription model for Garmin Connect. Let that sink in: You pay €800–€1,200 upfront... for lifetime access to robust features. Now they want MORE money for a subscription."

**Пост [836 очков]:**
> "I've cancelled my Instinct 3 Solar order. I don't wanna shell out 400€ and deal with subscription being forced on me in the future."

**Пост [1047 очков]:**
> "This is how it always starts... They will add more, if not all, new features in the subscription alternative."

**Вывод для ZERO:** Garmin сам убивает свою лояльную аудиторию. Наш pitch "купил один раз — работает всегда" попадает в открытую рану.

---

### Боль #2: Спасение жизни через Garmin
**Источник:** r/backpacking, 1494 очков
**Что произошло:** Опытный хайкер застрял в зыбучих песках в Arches NP. Спасён через Garmin inReach SOS.

> [Топ-комментарий, 216 очков]: "Without the Garmin this could have been a lot uglier. Good on you for being prepared."

> [Второй комментарий, 119]: "Cross post this to r/Garmin for quite the case study"

**Вывод для ZERO:** SOS кнопка — НЕ опциональная фича, это ОБЯЗАТЕЛЬНЫЙ элемент продукта. Люди платят за Garmin именно ради этого страхового случая.

---

### Боль #3: "Просто хочу связаться с семьёй в экстренной ситуации"
**Источник:** r/Meshtastic, пост "Is this accessible to idiots?" 37 очков

> "For years I have been trying to find a system to connect family members in case of a grid-down situation (Katrina, Goliath, Fern) or total SHTF. But we are so disparate in our abilities that I really need an accessible system. Is this it?"

**Вывод для ZERO:** Огромная аудитория хочет простое решение, но Meshtastic DIY — слишком сложно. ZERO должен быть "включи и работает".

---

### Боль #4: Надёжность антенн в лесу — главный технический вопрос
**Источник:** r/Meshtastic "So I did a test", 430 очков

> "Bigger or 'more dBi' is not better... High gain omnis have a flat-pizza-shaped radiation pattern. Often you don't want this. A resonant 5dBi is pretty much the optimum for everyone."
> "A test with more than 500m would be interesting. Lets say 5km."

**Вывод для ZERO:** Аудитория технически грамотна в части антенн. Маркетинг "100 км дальности" не работает — верят только реальным тестам в лесу. Делать видео тесты.

---

### Боль #5: "Не дублировать WiFi гаджет"
**Источник:** r/Ultralight, пост про Apple satellite messaging, 359 очков

> "Maybe not a replacement for a Garmin messenger, but still useful for iPhone users."
> [Вопрос]: "If iPhone adds satellite: are Garmin/SPOT and similar obsolete?"

**Вывод для ZERO:** Apple развивает satellite messaging (уже запущен). Это давление сверху. ZERO отвечает на это: "P2P mesh без спутника, без интернета, без Apple subscription. Работает всегда."

---

## 3. КОНКУРЕНТНАЯ КАРТА (обновлённая)

| Продукт | Цена | Подписка | GPS | Mesh | Носимый | Offline AI |
|---------|------|----------|-----|------|---------|------------|
| Garmin inReach Mini 2 | $350 | $12-65/мес | да (спутник) | нет | клипса | нет |
| Apple iPhone (satellite) | телефон | нет (пока) | нет (только SOS) | нет | телефон | нет |
| Meshtastic DIY T-Beam | $55-100 | нет | да (LoRa) | да | нет (коробка) | нет |
| GoTenna Pro X2 | $499 | нет | нет | да (2 узла) | клипса | нет |
| **ZERO v1** | **15-20K руб** | **нет** | **да (GPS+LoRa)** | **да** | **браслет** | **v2** |

**Ключевой вывод:** ZERO = единственный продукт в категории "носимый + mesh + GPS + без подписки"

---

## 4. РЕАЛЬНЫЕ ТЕХНИЧЕСКИЕ ОЖИДАНИЯ АУДИТОРИИ

Из анализа комментариев r/Meshtastic:

1. **Дальность в лесу** — 200-500м между нодами реалистично при деревьях; 2-5 км на открытом пространстве. Mesh компенсирует через хопы.
2. **Батарея** — T1000-E на 1100 mAh хватает на 1-2 дня с маяком каждые 15 сек
3. **Антенна** — 5 dBi оптимум для смешанной местности (лес + открытое)
4. **Протокол** — LongFast (SF11, BW250) = баланс дальности и скорости для группового трекинга
5. **Точность GPS** — T1000-E показывал точность до 200м в сложном рельефе за лесом

---

## 5. СЕГМЕНТЫ АУДИТОРИИ (уточнённые)

### Сегмент А — Outdoor Family (самый большой)
- Хайкеры с детьми/партнёром
- Горные туристы, кемперы
- Боль: потеря члена группы, нет связи
- Платёжеспособность: 15-25K за семью
- Пример: cruise пост (1969 очков), dog пост (451 очков)

### Сегмент Б — Prepper / Resilience
- Готовятся к блэкаутам, отключению интернета
- Покупают инфраструктурные решения
- Платёжеспособность: 20-40K
- Боль: "хочу систему для всей семьи но Meshtastic слишком сложный"

### Сегмент В — Охотники РФ (региональный №1)
- 40M в РФ, используют рации Baofeng/PMR
- Боль: нет GPS, нет координации группы, нет дальности
- Платёжеспособность: 10-15K за рацию замену
- Особенность: техника руками не страшат, но конфигурацию делать не хотят

### Сегмент Г — B2B / Профессионалы
- Заповедники, охотхозяйства, ЧОП, поисковые отряды
- Dinoken Wildlife Reserve кейс: $40 устройств на 160 км
- Платёжеспособность: 30-80K за устройство при объёме
- Особенность: нужна надёжность и поддержка

---

## 6. GARMIN УЯЗВИМОСТЬ — ОКНО ВОЗМОЖНОСТЕЙ

r/Garmin сейчас в огне:
- Топ пост 2026 года: "DO NOT sign up for Garmin Connect+" (11895 очков)
- Компания форсирует подписку на Fenix 7/8 ($800-1200+) владельцев
- Аудитория активно ищет альтернативы

**Что нужно сделать:** Reddit thread / пост в r/Garmin с альтернативой ZERO. Timing идеальный.

---

## 7. ФИНАЛЬНЫЕ РЕКОМЕНДАЦИИ

### MVP v1 Must Have (без этого не запускать):
1. LoRa Mesh работает из коробки — никаких настроек
2. GPS трекинг группы на офлайн карте (OpenStreetMap)
3. SOS кнопка (групповой сигнал тревоги)
4. Батарея 24+ часов
5. IP67 водозащита
6. Без подписки — навсегда

### Messaging для каждого сегмента:
- **Outdoor Family:** "Знайте где ваши близкие в любой точке леса. Без подписок."
- **Охотники РФ:** "Garmin стоит 35K + подписка в $. ZERO — 15K один раз. Без интернета. Без спутника."
- **Prepper:** "Когда у всех ноль — у тебя всё работает."
- **B2B:** "Meshtastic для профессионалов. Готовый продукт вместо DIY."

### Следующий шаг — валидация за $0:
1. Telegram канал @zero_band сегодня
2. Пост в r/Meshtastic: "Working on a wearable Meshtastic bracelet — would you buy?"
3. Пост в r/Garmin: "After rage-quitting Connect+, found an alternative approach..."
4. Таргет: 200 email/контактов до заказа T-Beam

---

*Файл: memory/projects/zero-band/market-research-v2.md*
*Источники: Reddit API (OAuth2), r/Meshtastic, r/backpacking, r/Garmin, r/preppers, r/Survival*
