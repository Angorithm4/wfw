"use strict";

function fixTime() {
  $('time.fixtime').each(function(){
    var date = moment($(this).text());
    if ($(this).hasClass('fixtime-second')) {
      $(this).text(date.format('YYYY-MM-DD HH:mm:ss'));
    } else if ($(this).hasClass('fixtime-short')) {
      $(this).text(date.format('M/D(ddd) HH:mm'));
    } else {
      $(this).text(date.format('YYYY-MM-DD(ddd) HH:mm'));
    }
    $(this).removeClass('fixtime');
  });
}
$(function() {
  // select2
  if ($('select').length) {
    $.fn.select2.defaults.set('theme', 'bootstrap');
    $('select').select2();
  }

  // collapse
  if($(window).width()<768){$('.panel-collapse').removeClass('in').collapse('hide');}
  $('.panel-heading').click(function(){$($(this).data('target')).collapse('toggle');});

  // panel-filter
  $('.filter-title').click(function() {
    if ($(this).hasClass('show')) {
      $(this).removeClass('show');
    } else {
      $(this).addClass('show');
    }
    $($(this).data('target')).slideToggle("fast");
  });

  // tooltip
  $('.tooltip-unix').each(function() {
    var unix = parseInt($(this).attr('title'), 10);
    if (1400000000 <= unix && unix <= 5000000000) {
      var date = new Date(unix*1000);
      $(this).attr('title', date.toLocaleString());
    }
  });
  $('[data-toggle="tooltip"]').tooltip();

  // toggle buttons
  $('.toggle-btn-text').click(function(){
    var state = ($(this).text() == $(this).data('on-text'));
    if (state) {
      $(this).text($(this).data('off-text'));
    } else {
      $(this).text($(this).data('on-text'));
    }
    // source code expand
    if ($(this).hasClass('source-code-expand-btn')) {
      var id = '#'+$(this).data('target');
      if (state) {
        $(id).css('max-height','none');
      } else {
        $(id).css('max-height','350px');
      }
    }
  });

  // fav button
  if ($('.fav-btn').length) {
    {
      reloadFavs();
      $('.fav-btn').each(function() {
        var name = $(this).data('name');
        if (favSet.has(name)) {
          $(this).attr('src', '//img.atcoder.jp/assets/icon/fav.png');
          return;
        }
      });
    }
    $('.fav-btn').click(function() {
      var name = $(this).data('name');
      if (toggleFav(name)) {
        $(this).attr('src', '//img.atcoder.jp/assets/icon/fav.png');
      } else {
        $(this).attr('src', '//img.atcoder.jp/assets/icon/unfav.png');
      }
    });
  }

  // tr data-href
  $('tbody tr[data-href]').addClass('clickable').click(function(e) {
    if (e.ctrlKey || e.metaKey) window.open($(this).attr('data-href'), '_blank');
    else window.location = $(this).attr('data-href');
  }).find('a').click(function(e) {
    e.stopPropagation();
  });

  // scroll page top
  {
    var spt = $('#scroll-page-top, #scroll-page-top-new');
    $(window).scroll(function() {
      if ($(this).scrollTop() > 500) {
        if (spt.is(':hidden')) spt.fadeIn();
      } else {
        if (!spt.is(':hidden')) spt.fadeOut();
      }
    });
    spt.click(function() {
      $('html,body').animate({scrollTop:0},200);
    });
  }

  // google analytics
  /// banner
  $('.ads-tracking').click(function() {
    ga('send', 'event', location.host+'::'+$(this).data('ads-segment'), 'click', $(this).attr('href'));
  });
});
