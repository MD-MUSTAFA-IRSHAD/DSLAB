<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1" />
  <title>MemoriJS — Single File Memory Task</title>
  <style>
    :root{--bg:#0f1724;--card:#0b1220;--accent:#7dd3fc;--muted:#9ca3af;--glass:rgba(255,255,255,0.04)}
    *{box-sizing:border-box}
    body{font-family:Inter, ui-sans-serif, system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial; margin:0; background:linear-gradient(180deg,#071022 0%, #071322 100%); color:#e6eef6; padding:28px;}
    .container{max-width:900px;margin:0 auto}
    header{display:flex;align-items:center;gap:16px;margin-bottom:18px}
    h1{margin:0;font-size:20px}
    p.lead{margin:0;color:var(--muted)}
    .card{background:linear-gradient(180deg, rgba(255,255,255,0.02), rgba(255,255,255,0.01)); padding:18px;border-radius:12px;box-shadow:0 6px 18px rgba(2,6,23,0.6);}
    .controls{display:flex;gap:12px;flex-wrap:wrap;margin-top:12px}
    label{font-size:13px;color:var(--muted)}
    select,input[type=number]{padding:8px 10px;border-radius:8px;border:none;background:var(--glass);color:inherit}
    button{background:var(--accent);border:none;color:#082029;padding:10px 12px;border-radius:10px;font-weight:600;cursor:pointer}
    .muted{color:var(--muted);font-size:13px}
    .stage{margin-top:18px;display:grid;grid-template-columns:1fr 340px;gap:14px}
    .screen{padding:18px;border-radius:10px;background:rgba(255,255,255,0.02);min-height:180px;display:flex;flex-direction:column;align-items:center;justify-content:center}
    .words{display:flex;gap:10px;flex-wrap:wrap;justify-content:center}
    .word{background:rgba(255,255,255,0.03);padding:12px 16px;border-radius:999px;font-weight:700}
    .countdown{font-size:42px;font-weight:800}
    textarea{width:100%;min-height:120px;padding:12px;border-radius:8px;border:none;background:var(--glass);color:inherit;resize:vertical}
    .result{margin-top:12px;padding:12px;border-radius:8px;background:rgba(0,0,0,0.25)}
    .flex{display:flex;gap:10px;align-items:center}
    .small{font-size:13px}
    .tag{display:inline-block;padding:6px 10px;border-radius:999px;background:rgba(255,255,255,0.03);font-weight:600}
    footer{margin-top:18px;color:var(--muted);font-size:13px}
    .btn-ghost{background:transparent;border:1px solid rgba(255,255,255,0.06);color:var(--muted)}
    .hidden{display:none}
    @media(max-width:880px){.stage{grid-template-columns:1fr;}}
  </style>
</head>
<body>
  <div class="container">
    <header>
      <div>
        <h1>MemoriJS — Single-file Memory Task</h1>
        <p class="lead">Show 3–5 words for a short time, hide them, then type what you remember.</p>
      </div>
      <div style="margin-left:auto" class="muted">SIH Demo Ready</div>
    </header>

    <div class="card">
      <div class="controls">
        <div>
          <label>Number of words</label><br>
          <select id="wordsCount">
            <option value="3">3</option>
            <option value="4">4</option>
            <option value="5" selected>5</option>
          </select>
        </div>
        <div>
          <label>Display duration (seconds)</label><br>
          <input id="displaySeconds" type="number" min="2" max="10" value="5">
        </div>
        <div>
          <label>Fuzzy match (allow small typos)</label><br>
          <select id="fuzzy">
            <option value="false" selected>Off</option>
            <option value="true">On (Levenshtein ≤ 1)</option>
          </select>
        </div>
        <div style="margin-left:auto; display:flex; gap:8px; align-items:end">
          <button id="startBtn">Start Test</button>
          <button id="demoBtn" class="btn-ghost">Demo words pool</button>
        </div>
      </div>

      <div class="stage">
        <div>
          <div class="screen" id="displayScreen">
            <div id="preStage">
              <p class="muted">Press <strong>Start Test</strong> to begin a trial. Words will appear for the set duration, then the prompt will appear.</p>
            </div>

            <div id="wordsStage" class="hidden">
              <div class="countdown" id="countdown"></div>
              <div style="height:12px"></div>
              <div class="words" id="wordsContainer"></div>
            </div>

            <div id="promptStage" class="hidden" style="width:100%">
              <p class="muted small">Type the words you remember. Separate by commas or new lines. Order does not matter.</p>
              <textarea id="recallInput" placeholder="e.g. apple, river, blue"></textarea>
              <div style="display:flex;gap:8px;margin-top:10px"><button id="submitBtn">Submit</button><button id="restartBtn" class="btn-ghost">Restart</button></div>
              <div id="timerSmall" class="muted small" style="margin-top:8px">Timer: <span id="responseTimer">0.00</span>s</div>
            </div>

            <div id="resultStage" class="hidden" style="width:100%">
              <h3 id="scoreHeader">Results</h3>
              <div id="resultBox" class="result"></div>
              <div style="display:flex;gap:8px;margin-top:10px"><button id="exportCsv">Export CSV</button><button id="tryAgain" class="btn-ghost">Try another</button></div>
            </div>
          </div>
        </div>

        <aside>
          <div class="card" style="padding:12px">
            <h4 style="margin:0 0 8px 0">Trial info</h4>
            <div class="small muted">Pool size:</div>
            <div id="poolCount" class="tag">--</div>

            <div style="margin-top:12px"><div class="muted small">Current words</div><div id="currentWords" class="muted"></div></div>

            <hr style="margin:12px 0;border:none;border-top:1px solid rgba(255,255,255,0.03)">
            <div class="muted small">Last trial</div>
            <div id="lastTrial" class="muted small">—</div>

            <hr style="margin:12px 0;border:none;border-top:1px solid rgba(255,255,255,0.03)">
            <div class="muted small">Tips</div>
            <ul style="padding-left:18px;margin:8px 0 0 0;color:var(--muted)">
              <li>Use short common words for fair recall</li>
              <li>Allow user consent if collecting demographics</li>
              <li>Fuzzy helps with small typos for demo settings</li>
            </ul>
          </div>
        </aside>
      </div>

      <footer>
        <div style="display:flex;gap:8px;align-items:center;justify-content:space-between">
          <div>Built for SIH — single-file demo. Open this HTML in any modern browser.</div>
          <div class="muted small">Tip: Press <strong>Start Test</strong> to run.</div>
        </div>
      </footer>
    </div>
  </div>

  <script>
    // --- Word pool (customize for your demo / add vernacular lists) ---
    const WORD_POOL = [
      'apple','river','table','green','music','phone','house','cloud','train','light',
      'moon','star','book','chair','glass','window','happy','ocean','paper','stone',
      'garden','clock','bread','water','mountain','flower','riverbank','yellow','circle','friend'
    ];

    // UI references
    const startBtn = document.getElementById('startBtn');
    const demoBtn = document.getElementById('demoBtn');
    const wordsCountSelect = document.getElementById('wordsCount');
    const displaySecondsInput = document.getElementById('displaySeconds');
    const fuzzySelect = document.getElementById('fuzzy');
    const wordsContainer = document.getElementById('wordsContainer');
    const countdownEl = document.getElementById('countdown');
    const wordsStage = document.getElementById('wordsStage');
    const preStage = document.getElementById('preStage');
    const promptStage = document.getElementById('promptStage');
    const recallInput = document.getElementById('recallInput');
    const submitBtn = document.getElementById('submitBtn');
    const restartBtn = document.getElementById('restartBtn');
    const resultStage = document.getElementById('resultStage');
    const resultBox = document.getElementById('resultBox');
    const scoreHeader = document.getElementById('scoreHeader');
    const poolCount = document.getElementById('poolCount');
    const currentWords = document.getElementById('currentWords');
    const lastTrial = document.getElementById('lastTrial');
    const responseTimer = document.getElementById('responseTimer');
    const exportCsv = document.getElementById('exportCsv');
    const tryAgain = document.getElementById('tryAgain');

    // State
    let trial = null; // {targetWords:[], startTs, submitTs, displaySeconds, fuzzy}
    let lastResult = null;
    let timerInterval = null;

    poolCount.textContent = WORD_POOL.length;

    function randPick(arr, n){
      const copy = arr.slice();
      const out = [];
      for(let i=0;i<n && copy.length;i++){
        const idx = Math.floor(Math.random()*copy.length);
        out.push(copy.splice(idx,1)[0]);
      }
      return out;
    }

    function showWords(words, seconds){
      wordsContainer.innerHTML = '';
      words.forEach(w=>{
        const el = document.createElement('div'); el.className='word'; el.textContent = w; wordsContainer.appendChild(el);
      });
      countdownEl.textContent = seconds;
      wordsStage.classList.remove('hidden'); preStage.classList.add('hidden'); promptStage.classList.add('hidden'); resultStage.classList.add('hidden');

      let remaining = seconds;
      const step = () => {
        remaining -= 1;
        countdownEl.textContent = remaining;
        if(remaining <= 0){
          clearInterval(timerInterval);
          hideWordsAndPrompt();
        }
      }
      timerInterval = setInterval(step, 1000);
    }

    function hideWordsAndPrompt(){
      wordsStage.classList.add('hidden');
      promptStage.classList.remove('hidden');
      recallInput.value = '';
      recallInput.focus();
      trial.startTs = performance.now();
      startResponseTimer();
    }

    function startResponseTimer(){
      const s = () => {
        const now = performance.now();
        const secs = ((now - trial.startTs)/1000).toFixed(2);
        responseTimer.textContent = secs;
      }
      s();
      timerInterval = setInterval(s, 100);
    }

    function stopResponseTimer(){
      clearInterval(timerInterval);
      trial.submitTs = performance.now();
      const secs = ((trial.submitTs - trial.startTs)/1000);
      responseTimer.textContent = secs.toFixed(2);
    }

    function normalizeList(str){
      return str.split(/[\n,]+/).map(s=>s.trim().toLowerCase()).filter(Boolean);
    }

    // Simple Levenshtein distance (fast small implementation)
    function levenshtein(a, b){
      if(a===b) return 0;
      const m = a.length, n = b.length;
      if(m===0) return n; if(n===0) return m;
      const row = new Array(n+1);
      for(let j=0;j<=n;j++) row[j]=j;
      for(let i=1;i<=m;i++){
        let prev = row[0]; row[0]=i;
        for(let j=1;j<=n;j++){
          const cur = row[j];
          const cost = a[i-1]===b[j-1] ? 0 : 1;
          row[j] = Math.min(prev+1, row[j]+1, row[j-1]+cost);
          prev = cur;
        }
      }
      return row[n];
    }

    function evaluateRecall(inputText){
      const guesses = normalizeList(inputText);
      const targetSet = new Set(trial.targetWords.map(t=>t.toLowerCase()));
      const fuzzy = fuzzySelect.value === 'true';

      const matched = [];
      const usedTargets = new Set();

      for(const g of guesses){
        // exact
        if(targetSet.has(g) && !usedTargets.has(g)){
          matched.push({guess:g,target:g,method:'exact'});
          usedTargets.add(g);
          continue;
        }
        if(fuzzy){
          // check any target within distance 1 not used yet
          let found = null;
          for(const t of trial.targetWords){
            const tl = t.toLowerCase();
            if(usedTargets.has(tl)) continue;
            const d = levenshtein(g, tl);
            if(d<=1){ found = tl; break; }
          }
          if(found){ matched.push({guess:g,target:found,method:'fuzzy'}); usedTargets.add(found); continue; }
        }
      }

      const correctCount = matched.length;
      const total = trial.targetWords.length;
      const timeTaken = ((trial.submitTs - trial.startTs)/1000).toFixed(2);

      // prepare annotated feedback
      const feedback = {
        guesses, matched, correctCount, total, timeTaken
      };
      return feedback;
    }

    function startTrial(){
      const n = parseInt(wordsCountSelect.value,10);
      const seconds = Math.max(2, Math.min(10, parseInt(displaySecondsInput.value,10) || 5));
      const pool = WORD_POOL;
      const selected = randPick(pool, n);
      trial = {targetWords: selected, displaySeconds: seconds, fuzzy: fuzzySelect.value};
      currentWords.textContent = selected.join(', ');
      // show stage
      showWords(selected, seconds);
    }

    function submitRecall(){
      stopResponseTimer();
      const text = recallInput.value;
      const feedback = evaluateRecall(text);
      lastResult = {...feedback, timestamp: new Date().toISOString(), trial: trial};
      renderResult(lastResult);
    }

    function renderResult(res){
      promptStage.classList.add('hidden');
      resultStage.classList.remove('hidden');
      scoreHeader.textContent = Score: ${res.correctCount} / ${res.total}    —    ${res.timeTaken}s;
      let html = '';
      html += <div><strong>Target words</strong>: ${res.trial.targetWords.join(', ')}</div>;
      html += <div style="margin-top:8px"><strong>Your guesses</strong>: ${res.guesses.join(', ') || '—'}</div>;
      if(res.matched.length){
        html += <div style="margin-top:8px"><strong>Matched</strong>: `+ res.matched.map(m=>${m.guess} → ${m.target} (${m.method})).join(', ') +</div>`;
      }
      if(res.guesses.length && res.guesses.length>res.matched.length){
        const wrong = res.guesses.filter(g=>!res.matched.some(m=>m.guess===g));
        html += <div style="margin-top:8px;color:#fca5a5"><strong>Incorrect</strong>: ${wrong.join(', ')}</div>;
      }
      html += <div style="margin-top:12px"><strong>Time</strong>: ${res.timeTaken}s</div>;
      html += <div style="margin-top:8px;color:var(--muted)"><em>Result timestamp: ${res.timestamp}</em></div>;

      resultBox.innerHTML = html;
      lastTrial.textContent = ${res.correctCount}/${res.total} — ${res.timeTaken}s;
    }

    function exportLastToCsv(){
      if(!lastResult) return alert('No result to export yet.');
      const rows = [];
      const head = ['timestamp','target_words','guesses','matched_pairs','correct_count','total','time_s','display_seconds','fuzzy'];
      rows.push(head.join(','));
      const matchedPairs = lastResult.matched.map(m=>${m.guess}->${m.target}).join('|');
      const line = [lastResult.timestamp, '"'+lastResult.trial.targetWords.join('|')+'"', '"'+lastResult.guesses.join('|')+'"','"'+matchedPairs+'"', lastResult.correctCount, lastResult.total, lastResult.timeTaken, lastResult.trial.displaySeconds, lastResult.trial.fuzzy];
      rows.push(line.map(cell=>cell).join(','));
      const csv = rows.join('\n');
      const blob = new Blob([csv], {type:'text/csv'});
      const url = URL.createObjectURL(blob);
      const a = document.createElement('a'); a.href = url; a.download = 'memori_result.csv'; document.body.appendChild(a); a.click(); a.remove();
      URL.revokeObjectURL(url);
    }

    // --- Event wiring ---
    startBtn.addEventListener('click', ()=>{
      startTrial();
    });
    demoBtn.addEventListener('click', ()=>{
      alert('Demo pool preview:\n\n' + WORD_POOL.join(', '));
    });
    submitBtn.addEventListener('click', ()=>{
      submitRecall();
    });
    restartBtn.addEventListener('click', ()=>{
      // reset to pre stage
      clearInterval(timerInterval);
      preStage.classList.remove('hidden'); wordsStage.classList.add('hidden'); promptStage.classList.add('hidden'); resultStage.classList.add('hidden'); responseTimer.textContent = '0.00';
    });
    exportCsv.addEventListener('click', exportLastToCsv);
    tryAgain.addEventListener('click', ()=>{ clearInterval(timerInterval); preStage.classList.remove('hidden'); resultStage.classList.add('hidden'); wordsStage.classList.add('hidden'); recallInput.value=''; responseTimer.textContent='0.00'; });

    // Accessibility: allow Enter in textarea with Ctrl+Enter to submit
    recallInput.addEventListener('keydown', (e)=>{
      if(e.key === 'Enter' && (e.ctrlKey || e.metaKey)){
        e.preventDefault(); submitRecall();
      }
    });

    // initial UI
    poolCount.textContent = WORD_POOL.length;
    currentWords.textContent = '—';

  </script>
</body>
</html>
