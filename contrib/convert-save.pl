#!/usr/bin/perl
use Data::Dumper;
sub hashenum {
  open F,$_[0] or die $!;
  $foo=join "",<F>;
  close F;
#$foo =~ s,\n+,\n,mg;
  (%enums)=$foo =~ m,\nenum\s+([^{\s]+)\s*{\s*\n([^}]+)},sg;
  #print Dumper(\%enums);
  my %ret;
  foreach my $k (keys(%enums)) {
    my $v=$enums{$k};
#$v =~ s,^\s+//.+$,,mg;
    $v =~ s,/\*.+?\*/,,sg;
    $v =~ s,//.*$,,mg;
    $v=~ s,\s*\n$,,s;
#print "$v\n\n";
    my @i=split(/[\s\n]*,[\s\n]*/,$v);
    my $c=0;
    foreach(@i) {
      $ret{$k}{bynum}{$c}=$_;
      $ret{$k}{bystr}{$_}=$c;
      print "$k \t ".$c." \t $_\n" if($ARGV[1] eq '');
      $c++;
    }
  }
  return %ret;
}

sub compare {
  my %h1=hashenum $_[0];
  my %h2=hashenum $_[1];
  my $ret;
  foreach my $en (keys(%h2)) {
    foreach my $num (keys(%{$h2{$en}{bynum}})) {
      if($h1{$en}{bynum}{$num} ne "") {
        my $k1=$h1{$en}{bynum}{$num};
        my $k2=$h2{$en}{bynum}{$num};
        if($h1{$en}{bynum}{$num} ne $h2{$en}{bynum}{$num}) {
  #@         print "MISSING!!!!\n" if($h2{$en}{bystr}{$k1} eq "");
           print $en." \t $k1 \t ".$num." => ".($h2{$en}{bystr}{$k1}||"NEW")."\t".($h2{$en}{bystr}{$k1}-$num)."\n";
           $ret{$en}{$num}=$h2{$en}{bystr}{$k1};
        }
      }
    }
  }
  return %ret;
}

sub convert_omaps {
#  chdir 'save' or die $1;
  foreach(<o.*>) {
    if(! -e "old-$_") {
      print "$_\n";
      rename "$_","old-$_" or die "$!";
      open NEW,">$_" or die "$!";
      open OLD,"<old-$_" or die "$!";
      my $start=1;
      while (my $l=<OLD>) {
        $start=0 if($l =~ /^Z /);
             if($start) {
               my (@ids)=$l =~ m,(\d+),g;
               if($#ids==179) {
                 my @newids;
                 my $ch=0;
                 foreach my $i (@ids) {
                     push @newids,($remap{oter_id}{$i} ne "" ? $remap{oter_id}{$i} : $i);
                 }
                 $l=join(" ",@newids)." \n";
               }
             }
        print NEW $l;
      }
      close OLD;
      close NEW;
    }
  }
}


sub convert_maps {
  if (! -e "old-maps.txt") {
    rename "maps.txt","old-maps.txt" or die "$!";
    open NEW,">maps.txt" or die "$!";
    open OLD,"old-maps.txt";
    $start=1;
    $inv=0;
    my $cm=0;my $cv=0;my $cf=0;
    while(my $l=<OLD>) {
      if ($l =~ /^----/) {
          $start=0; $inv=0;
      } else {
        $start++;
        if($start <= 12) {
           my (@ids)=$l =~ m,(\d+),g;
           if($#ids==11) {
             my @newids;
             my $ch=0;
             foreach my $i (@ids) {
                 push @newids,($remap{ter_id}{$i} ne "" ? $remap{ter_id}{$i} : $i);
                 if ($remap{ter_id}{$i} ne "") { $cm++; };
             }
             $l=join(" ",@newids)." \n";
             #$start++;
           }
        } elsif($start > 12) {
          if($l =~ /^V/) {
            $inv=1;
          } elsif ($l =~ /^F /) {
 print "f: $l";
            my ($f,$ft,$e) = $l =~ /^(F \d+ \d+ )(\d+)( \d+ \d+)/;
            if ($remap{field_id}{$ft} ne "") {
              $cf++;
              $l=$f.$remap{field_id}{$ft}.$e."\n";
            }
          } elsif ($inv && $l =~ /^\d+ /) {
            my (@ids)=$l =~ m,(-?\d+),g;
            if($#ids==7) {
  #print $l;
              $ids[0]=($remap{vpart_id}{$ids[0]} ne "" ? $remap{vpart_id}{$ids[0]} : $ids[0]);
                 if ($remap{vpart_id}{$i} ne "") { $cv++; };

              $l=join(" ",@ids)."\n";
  #print $l."\n";
            }
          }
        }
      }
      print NEW $l;
      
    }
    close OLD;
    close NEW;

print "$cm $cv $cf\n";

  }
}

use Cwd 'abs_path';
#print abs_path("..");
if($ARGV[1] ne '') {
if ( -d "$ARGV[0]" && -d "$ARGV[1]" ) {
$odir=abs_path ("$ARGV[1]");
$ndir=abs_path ("$ARGV[0]");
%remap=(
  compare("$odir/omdata.h", "$ndir/omdata.h")
  ,compare("$odir/mapdata.h", "$ndir/mapdata.h")
  ,compare("$odir/veh_type.h", "$ndir/veh_type.h")
#  ,compare("$odir/pldata.h", "$ndir/pldata.h")
);

#print Dumper(\%remap);
foreach(keys(%remap)) {
  my $c=keys(%{$remap{$_}});
  print "$_: $c\n";
}
if ($ARGV[2] =~ /y/) {
chdir "$odir" or die "$!";
my @fail;
foreach("$odir/omdata.h", "$ndir/omdata.h","$odir/mapdata.h", "$ndir/mapdata.h",
  "$odir/veh_type.h", "$ndir/veh_type.h",
# unimplemented  "$odir/pldata.h", "$ndir/pldata.h",
  "maps.txt") {
  push @fail,$_ if ( ! -e $_ );
}
die "No @fail" if ($#fail > 0);
convert_omaps;
convert_maps;
print `cp -av $ndir/omdata.h $ndir/mapdata.h $ndir/veh_type.h .`;
#`cp -a $ndir/
}
exit;
}
# c65458
  %remap=compare $ARGV[0],$ARGV[1];# "../c2/omdata.h","./omdata.h";

#  print Dumper($remap{oter_id});
  convert_save2 if ($ARGV[2] =~ /o/);;
  convert_save_maps if ($ARGV[2] =~ /m/);;
#die Dumper(\%remap);

  convert_save_maps_v if ($ARGV[2] =~ /v/);;
  
} elsif ($ARGV[0] ne '') {
  hashenum $ARGV[0];
} else { 
  print "Usage... $0 <new-source-code> <save-dir>\n   <save-dir> must contain omdata.h, mapdata.h, and veh_type.h from the source code directory of the version that saved it. This script will place these files in the save directory after a successful run, and save original files as old-*. You must delete old-whatever to update the file again.\n";#  hashenum "./omdata.h";
}


