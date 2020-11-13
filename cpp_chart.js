
function ready(callback) {
    if (document.readyState !== 'loading') callback();
    else if (document.addEventListener) document.addEventListener('DOMContentLoaded', callback);
    else document.attachEvent('onreadystatechange', function () {
        if (document.readyState === 'complete') callback();
    });
}

ready(() => {
    var varData = {};
    var datas = [];

    function buildVarData(data, showGroups, hideGroups) {
        data.forEach((d) => {
            d.offsets = d.samples.map(s => ({
                time: s.time - d.baseline.time,
                ram: s.ram - d.baseline.ram
            }));
            let x = d.sourceFile.match(/\/([^\/]+)\.cpp/)[1];
            d.key = '_' + x;
            d.label = d.graphLabel || x.replace(/_/g, ' ');
        });
        data.sort((a, b) => d3.ascending(a.inputNumber, b.inputNumber));

        let groups = d3.nest().key(d => d.key).entries(data);

        let color = d3.scaleOrdinal()
            .domain(groups.map(g => g.key))
            .range(['#4e79a7', '#e28e2c', '#e15759', '#76b7b2', '#59a14f', '#ddb939', '#af7aa1', '#ff9da7', '#9c755f', '#bab0ab', '#e41a1c', '#377eb8', '#4daf4a', '#984ea3']);

        let groupKeys = groups.map(g => g.key);
        let hiddenKeys = hideGroups;
        if (showGroups.length != 0) {
            hiddenKeys = groupKeys.filter(k => !showGroups.includes(k));
        }

        let updates = [];
        let elems = [];
        function updateHidden() {
            let base = d3.selectAll(elems);
            if (hiddenKeys.length > 0)
                base.selectAll(hiddenKeys.map(k => "." + k).join(','))
                    .transition()
                    .style("opacity", 0)
                    .on("end", function () {
                        d3.select(this).style("display", "none");
                    });
            var shownKeys = groupKeys.filter(k => !hiddenKeys.includes(k));
            if (shownKeys.length > 0)
                base.selectAll(shownKeys.map(k => "." + k).join(','))
                    .style("display", null)
                    .transition()
                    .style("opacity", 1);
        }

        function updateAll() {
            updates.forEach((v) => v());
            updateHidden();
        }

        function toggleHidden(key) {
            let hidden = hiddenKeys.includes(key);
            if (hidden)
                hiddenKeys.splice(hiddenKeys.indexOf(key), 1);
            else
                hiddenKeys.push(key);
            updateAll();
        }

        let toggles = {
            withOffset: true,
            divideInputNumber: false,
        };

        function toggleWithOffset() {
            toggles.withOffset = !toggles.withOffset;
            updateAll();
        }
        function toggleDivideInputNumber() {
            toggles.divideInputNumber = !toggles.divideInputNumber;
            updateAll();
        }

        return {
            groups: groups,
            color: color,
            updates: updates,
            updateAll: updateAll,
            elems: elems,
            groupKeys: groupKeys,
            hiddenKeys: hiddenKeys,
            updateHidden: updateHidden,
            toggleHidden: toggleHidden,
            toggles: toggles,
            toggleWithOffset: toggleWithOffset,
            toggleDivideInputNumber: toggleDivideInputNumber,
        };
    }

    let margin = { top: 10, right: 30, bottom: 50, left: 100 },
        width = 800 - margin.left - margin.right,
        height = 400 - margin.top - margin.bottom;

    for (let elem of document.querySelectorAll('.sampleChart')) {
        let div = d3.select(elem);
        let varName = elem.getAttribute('data-var');
        let key = elem.getAttribute('data-key');
        let label = elem.getAttribute('data-label');

        let data = window[varName];
        if (!(d3.max(data, d => d.baseline[key]) > 0)) continue;

        if (!(varName in varData)) {
            let showGroups = (elem.getAttribute('data-show-groups') || "").split(',').filter(e => e.length != 0).map(k => '_' + k);
            let hideGroups = (elem.getAttribute('data-hide-groups') || "").split(',').filter(e => e.length != 0).map(k => '_' + k);
            varData[varName] = buildVarData(data, showGroups, hideGroups);
            datas.push(varData[varName]);
        }
        let env = varData[varName];
        let groups = env.groups;
        let color = env.color;

        let svg = div.append("svg")
            .attr("width", width + margin.left + margin.right)
            .attr("height", height + margin.top + margin.bottom)
            .append("g")
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

        // xAxis
        let x = d3.scaleLinear()
            .domain([0, d3.max(data, d => d.inputNumber)])
            .range([0, width]);
        svg.append("g")
            .attr("transform", "translate(0," + height + ")")
            .call(d3.axisBottom(x));
        svg.append("text")
            .attr("text-anchor", "end")
            .attr("x", width)
            .attr("y", height + margin.top + 25)
            .text("Input Number");

        // yAxis
        let y = d3.scaleLinear()
            .range([height, 0]);
        let yAxis = svg.append("g");
        svg.append("text")
            .attr("text-anchor", "end")
            .attr("transform", "rotate(-90)")
            .attr("y", -margin.left + 25)
            .attr("x", -margin.top)
            .text(label);

        let all = svg.selectAll("groups")
            .data(groups)
            .enter();

        // range
        let ranges = all.append("g")
            .attr("class", g => g.key)
            .append("path")
            .attr("fill", "#cce5df")
            .attr("opacity", 0.6)
            .attr("stroke", "none");

        // line along median
        let lines = all.append("g")
            .attr("class", g => g.key)
            .append("path")
            .attr("fill", "none")
            .attr("stroke", g => color(g.key))
            .attr("stroke-width", 1.5);

        // tooltip
        let tooltip = div.append("div")
            .style("opacity", 0)
            .attr("class", "tooltip");

        let mouseover = () => { tooltip.style("display", null).transition().style("opacity", 1); };
        let mouseleave = () => { tooltip.style("display", "none").style("opacity", 0); };
        let locale = 'de-DE';
        let mousemove = function (d) {
            let html = `<b>${d.label}</b><br>
          inputNumber: ${d.inputNumber.toLocaleString(locale)}<br>
          time: ${d3.median(d.samples, s => s.time).toLocaleString(locale)}s (${d3.median(d.offsets, s => s.time).toLocaleString(locale)}s)<br>`;
            if (d.baseline.ram > 0)
                html += `ram: ${d3.median(d.samples, s => s.ram).toLocaleString(locale)}kb (${d3.median(d.offsets, s => s.ram).toLocaleString(locale)}kb)<br>`;
            html += `<br>${d.samples.length.toLocaleString(locale)} samples:<br><ul>
          <li>${d.samples.map(s => `${s.time.toLocaleString(locale)}s (${(s.time - d.baseline.time).toLocaleString(locale)}s)`).join('<li>')}</ul>`;
            tooltip.html(html)
                .style("left", (d3.mouse(this)[0] + 74 + 8. / window.devicePixelRatio) + "px")
                .style("top", (d3.mouse(this)[1] + 16 + 16. / window.devicePixelRatio) + "px");
        };

        // points
        let points = all.append("g")
            .attr("class", g => g.key)
            .selectAll("points")
            .data(g => g.values)
            .enter()
            .append("circle")
            .attr("class", "dataPoint")
            .attr("cx", d => x(d.inputNumber))
            .attr("r", 4)
            .attr("stroke", "white")
            .on("mouseover", mouseover)
            .on("mouseleave", mouseleave)
            .on("mousemove", mousemove);

        // legend
        let legends = svg.selectAll("legend")
            .data(groups)
            .enter()
            .append("g")
            .append("text")
            .attr("text-anchor", "start")
            .attr("y", (g, i) => 10 + i * 15)
            .attr("x", 10)
            .text(g => g.values[0].label)
            .style("fill", g => color(g.key))
            .attr("class", "graphLabel")
            .on("click", g => env.toggleHidden(g.key));

        // offset control
        let controls = svg.append("g")
            .attr("transform", "translate(0," + (height + 25) + ")");
        let offsetCircle = controls.append("circle")
            .attr("class", "offsetCircle")
            .attr("cx", 7)
            .attr("cy", 7)
            .attr("r", 7)
            .attr("stroke", "black")
            .on("click", env.toggleWithOffset);
        controls.append("text")
            .attr("text-anchor", "start")
            .attr("x", 17)
            .attr("y", 12)
            .text("Subtract Baseline")
            .on("click", env.toggleWithOffset);

        let divideCircle = controls.append("circle")
            .attr("class", "divideCircle")
            .attr("cx", 207)
            .attr("cy", 7)
            .attr("r", 7)
            .attr("stroke", "black")
            .on("click", env.toggleDivideInputNumber);
        controls.append("text")
            .attr("text-anchor", "start")
            .attr("x", 217)
            .attr("y", 12)
            .text("per Input Number")
            .on("click", env.toggleDivideInputNumber);

        function update() {
            legends.text(g => (env.hiddenKeys.includes(g.key) ? '☐ ' : '☑ ') + g.values[0].label);

            let toggles = env.toggles;
            let shownData = data.filter(g => !env.hiddenKeys.includes(g.key));
            let divider = d => toggles.divideInputNumber ? d.inputNumber : 1;
            let yMedian = d => d3.median(toggles.withOffset ? d.offsets : d.samples, s => s[key] / divider(d));
            let yMax = d => d3.max(toggles.withOffset ? d.offsets : d.samples, s => s[key] / divider(d));
            let yMin = d => d3.min(toggles.withOffset ? d.offsets : d.samples, s => s[key] / divider(d));
            if (shownData.length > 0) {
                y.domain([Math.min(0, d3.min(shownData, yMin)), d3.max(shownData, yMax) * 1.05]);
                yAxis.transition().call(d3.axisLeft(y));
            }

            ranges.transition().attr("d", g => d3.area()
                .x(d => x(d.inputNumber)).y0(d => y(yMax(d))).y1(d => y(yMin(d)))
                (g.values));
            lines.transition().attr("d", g => d3.line()
                .x(d => x(d.inputNumber)).y(d => y(yMedian(d)))
                (g.values));

            points.transition().attr("cy", d => y(yMedian(d)));
            offsetCircle.transition().attr("fill", toggles.withOffset ? "black" : "white");
            divideCircle.transition().attr("fill", toggles.divideInputNumber ? "black" : "white");
        }
        env.elems.push(elem);
        env.updates.push(update);
    }

    for (let elem of document.querySelectorAll('.valueChart')) {
        let div = d3.select(elem);
        let varName = elem.getAttribute('data-var');
        let key = elem.getAttribute('data-key');
        let label = elem.getAttribute('data-label');

        let data = window[varName];
        if (!(d3.max(data, d => d.astPrint[key]) > 0)) continue;

        if (!(varName in varData)) {
            varData[varName] = buildVarData(data);
            datas.push(varData[varName]);
        }
        let env = varData[varName];
        let groups = env.groups;
        let color = env.color;

        let svg = div.append("svg")
            .attr("width", width + margin.left + margin.right)
            .attr("height", height + margin.top + margin.bottom)
            .append("g")
            .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

        // xAxis
        let x = d3.scaleLinear()
            .domain([0, d3.max(data, d => d.inputNumber)])
            .range([0, width]);
        svg.append("g")
            .attr("transform", "translate(0," + height + ")")
            .call(d3.axisBottom(x));
        svg.append("text")
            .attr("text-anchor", "end")
            .attr("x", width)
            .attr("y", height + margin.top + 25)
            .text("Input Number");

        // yAxis
        let y = d3.scaleLinear()
            .range([height, 0]);
        let yAxis = svg.append("g");
        svg.append("text")
            .attr("text-anchor", "end")
            .attr("transform", "rotate(-90)")
            .attr("y", -margin.left + 25)
            .attr("x", -margin.top)
            .text(label);

        let all = svg.selectAll("groups")
            .data(groups)
            .enter();

        // line
        let lines = all.append("g")
            .attr("class", g => g.key)
            .append("path")
            .attr("fill", "none")
            .attr("stroke", g => color(g.key))
            .attr("stroke-width", 1.5);

        // tooltip
        let tooltip = div.append("div")
            .style("opacity", 0)
            .attr("class", "tooltip");

        let mouseover = () => { tooltip.style("display", null).transition().style("opacity", 1); };
        let mouseleave = () => { tooltip.style("display", "none").style("opacity", 0); };
        let locale = 'de-DE';
        let mousemove = function (d) {
            let html = `<b>${d.label}</b><br>
          inputNumber: ${d.inputNumber.toLocaleString(locale)}<br>
          time: ${d3.median(d.samples, s => s.time).toLocaleString(locale)}s (${d3.median(d.offsets, s => s.time).toLocaleString(locale)}s)<br>`;
            if (d.baseline.ram > 0)
                html += `ram: ${d3.median(d.samples, s => s.ram).toLocaleString(locale)}kb (${d3.median(d.offsets, s => s.ram).toLocaleString(locale)}kb)<br>`;
            html += `<br>${d.samples.length.toLocaleString(locale)} samples:<br><ul>
          <li>${d.samples.map(s => `${s.time.toLocaleString(locale)}s (${(s.time - d.baseline.time).toLocaleString(locale)}s)`).join('<li>')}</ul>`;
            tooltip.html(html)
                .style("left", (d3.mouse(this)[0] + 74 + 8. / window.devicePixelRatio) + "px")
                .style("top", (d3.mouse(this)[1] + 16 + 16. / window.devicePixelRatio) + "px");
        };

        // points
        let points = all.append("g")
            .attr("class", g => g.key)
            .selectAll("points")
            .data(g => g.values)
            .enter()
            .append("circle")
            .attr("class", "dataPoint")
            .attr("cx", d => x(d.inputNumber))
            .attr("r", 4)
            .attr("stroke", "white")
            .on("mouseover", mouseover)
            .on("mouseleave", mouseleave)
            .on("mousemove", mousemove);

        // legend
        let legends = all.append("g")
            .append("text")
            .attr("text-anchor", "start")
            .attr("y", (g, i) => 10 + i * 15)
            .attr("x", 10)
            .text(g => g.values[0].label)
            .style("fill", g => color(g.key))
            .style("font-size", 12)
            .on("click", g => env.toggleHidden(g.key));

        // offset control
        let controls = svg.append("g")
            .attr("transform", "translate(0," + (height + 25) + ")");
        let offsetCircle = controls.append("circle")
            .attr("class", "offsetCircle")
            .attr("cx", 7)
            .attr("cy", 7)
            .attr("r", 7)
            .attr("stroke", "black")
            .on("click", env.toggleWithOffset);
        controls.append("text")
            .attr("text-anchor", "start")
            .attr("x", 17)
            .attr("y", 12)
            .text("Subtract Baseline")
            .on("click", env.toggleWithOffset);

        function update() {
            legends.text(g => (env.hiddenKeys.includes(g.key) ? '☐ ' : '☑ ') + g.values[0].label);

            let toggles = env.toggles;
            let shownData = data.filter(g => !env.hiddenKeys.includes(g.key));
            let divider = (d) => toggles.divideInputNumber ? d.inputNumber : 1;
            let yValue = (d) => (toggles.withOffset ? (d.astPrint[key] - d.astPrint.baseline[key]) : d.astPrint[key]) / divider(d);
            if (shownData.length > 0) {
                y.domain([0, d3.max(shownData, yValue) * 1.05]);
                yAxis.transition().call(d3.axisLeft(y));
            }
            lines.transition().attr("d", g => d3.line()
                .x(d => x(d.inputNumber))
                .y(d => y(yValue(d)))
                (g.values)
            );
            points.transition().attr("cy", d => y(yValue(d)));
            offsetCircle.transition().attr("fill", toggles.withOffset ? "black" : "white");
        }
        env.elems.push(elem);
        env.updates.push(update);
    }

    for (let d of datas) d.updateAll();
});
