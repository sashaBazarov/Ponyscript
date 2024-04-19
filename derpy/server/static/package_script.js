
function copyCommand() {
    var commandInput = document.getElementById('commandInput');
    commandInput.select();
    document.execCommand('copy');
    alert('Command copied to clipboard');
}


var socket = io();
var searchinput = document.getElementById("searchInput");
function search_input(){

    // Обработчик клика на результат поиска
  $('#searchResultsList').on('click', 'li', function(){
    var resultText = $(this).text();
    
    window.location.href = "/package?name=" + resultText;

    // Здесь можно добавить дополнительную логику при клике на результат
  });

  FindLibs(searchinput.value)

  $('#searchResults').css({
    display: 'block',
    top: $('#searchInput').offset().top + $('#searchInput').outerHeight() - 1, // Компенсация для линии подчеркивания input
    left: $('#searchInput').offset().left,
    width: $('#searchInput').outerWidth() // Установка ширины всплывающего окна равной ширине input
  });



  // Закрыть всплывающее окно при клике вне его области
  $(document).on('click', function(e){
    if(!$(e.target).closest('#searchResults').length && !$(e.target).is('#searchInput')) {
      $('#searchResults').css('display', 'none');
    }
  });
}

function FindLibs(text) {
  if(text == ""){
      block.innerHTML = "";
  }else{
      socket.emit("FindLibs", text)
  }
  }

socket.on("getlib", function(data) {
console.log(data)
var searchResults = document.getElementById("searchResultsList");
var content = "";
for (var package of data) {
  content += '<li> '+ package[0] +' </li>';
}
searchResults.innerHTML = content;
});
