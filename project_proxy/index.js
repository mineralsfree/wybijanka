const textInput = document.getElementById('text');
const form = document.getElementById('form');
const root = document.getElementById('root');
const textarea = document.getElementById('textarea');
const turn = document.getElementById('turn')
const ws = new WebSocket("ws://localhost:8080");

const resolveLetter = (num)=>{
  return String.fromCharCode(num + 'A'.charCodeAt(0));
}
ws.onopen = function open(){
  console.log('opened');
}
const render = (data)=>{
  window.selected = ''
  const rows = data.split('\n').map(el=>el.split(''));
  let html = '';
  for (let i = 65; i <65+8 ; i++) {
    html+=`<div class="square text">${String.fromCharCode(i)}</div>`
  }
  html+=`<div class="square text"></div>`
  const selectMovement = (e)=>{
    console.log(window.selected)
    if (window.selected.length !== 2 ){
      return
    } else {
      window.selected+=resolveLetter(Number(e.target.id.split(';')[0]));
      window.selected+= e.target.id.split(';')[1];
      ws.send(window.selected);
      turn.innerText = '';
    }
  }
  const selectD = (e)=>{
    if (window.selected.length === 2){
      window.selected+=resolveLetter(Number(e.target.id.split(';')[0]));
      window.selected+= e.target.id.split(';')[1];
      ws.send(window.selected);
      turn.innerText = '';

    } else {
      window.selected=resolveLetter(Number(e.target.id.split(';')[0]))+ e.target.id.split(';')[1];
    }
    console.log(window.selected);
  }
  let color;
  rows.forEach((row, i)=>{
    row.forEach((el, j)=>{
      if (i%2 === j%2){
        color = 'white'
      } else {
        color = 'black'
      }
      if (el === '-'){
        html+=`<div class='square ${color} empty'  id="${j + ';'+(i+1)}"></div>`
      } else if (el === 'w'){
        html+=`<div class='square ${color} white-draught draught' id="${j + ';'+(i+1)}"></div>`;
      } else if (el === 'b'){
        html+=`<div class='square ${color} black-draught draught' id="${j + ';'+(i+1)}"></div>`;
      } else if (el === 'W'){
        html+=`<div class='square ${color} white-draught-K draught' id="${j + ';'+(i+1)}"></div>`;
      } else if (el === 'B'){
        html+=`<div class='square ${color} black-draught-K draught' id="${j + ';'+(i+1)}"></div>`;

      }

    })

    html+= i<8?`<div class='square text'>${i+1}</div>`:'';
  })
  root.innerHTML = html;
  const draughts = document.querySelectorAll('.draught');
  draughts.forEach(draught=>{
    draught.addEventListener('click', selectD);
  })
  const emptys = document.querySelectorAll('.empty');
  emptys.forEach(empty => {
    empty.addEventListener('click',selectMovement )
  })


}
ws.onmessage = function message(event){
  console.log(event.data)
  textarea.value+= event.data
  if (event.data.startsWith('f')){
    render(event.data.split('f')[1]);
  } else if (event.data.startsWith('color')){
   const colorElement = document.getElementById('color');
   colorElement.innerText = event.data;
  } else if (event.data.startsWith('your')){
    turn.innerText = 'IT\'S YOUR TURN'
  } else if (event.data.startsWith('won!')){
    alert(event.data.split(' ').reverse().join(' ') + 'Reload page to try one more time');
  }
};
form.onsubmit = (e)=>{
  e.preventDefault();
  const text = textInput.value;
  ws.send(text);
}
