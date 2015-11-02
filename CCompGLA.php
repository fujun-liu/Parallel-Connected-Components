<?
function Connected_Components(array $t_args, array $inputs, array $outputs)
{
    // Class name is randomly generated
    $className = generate_name("CCompGLA");

    // The dimension of the data, i.e. how many elements are in each item.
    $dimension = count($inputs);

    // Array for generating inline C++ code;
    $codeArray = array_combine(array_keys($inputs), range(0, $dimension - 1));

    // Setting output type
    for ($counter = 0; $counter < 5 * $dimension; $counter++)
      array_set_index($outputs, $counter, lookupType("base::DOUBLE"));
    array_set_index($outputs, $counter, lookupType("base::INT"));

    $names = array_keys($outputs);
    $variables = ["mean", "std_dev", "range", "min", "max"];
    $count = array_pop($names);

    $sys_headers = ['unordered_map', 'set'];
    $user_headers = ['UnionFindMap.h'];
    $lib_headers = [];
?>

using namespace std;

class <?=$className?>;

class <?=$className?> {
 private:
  // union-find map data structure, which contains nodeID->compID information
  UnionFindMap localUF, globalUF;
  std::unordered_map<long, long>::iterator OutputIterator, EndOfOutput;
  bool localFinalized = false;
  long item[<?=$dimension?>];
 public:
  <?=$className?>() {}

  void AddItem(<?=const_typed_ref_args($inputs)?>) {
<?  foreach ($i = 0;  $i < $dimension; ++ $i) { ?>
    item[<?=$counter?>] = <?=$name?>;
<?  } ?>
    if (any(item != item))
      cout << "nan seen" << endl;
    sum += item;
    sum_squared += item % item;
    min = arma::min(min, item);
    max = arma::max(max, item);
    count++;
  }

  void AddState(<?=$className?> &other) {
    sum += other.sum;
    sum_squared += other.sum_squared;
    min = arma::min(min, other.min);
    max = arma::max(max, other.max);
    count += other.count;
  }

  void GetResult(<?=typed_ref_args($outputs)?>) {
    vec::fixed<<?=$dimension?>> mean(sum / count);
    vec::fixed<<?=$dimension?>> var(sum_squared / count - mean % mean);
    var *= 1.0 + 1.0 / (count - 1);
    vec::fixed<<?=$dimension?>> std_dev(sqrt(var));
    vec::fixed<<?=$dimension?>> range(max - min);
    cout << "sum" << endl << sum << endl;
    cout << "count" << endl << count << endl;
    cout << "mean" << endl << mean << endl;
<?  foreach ($names as $counter => $output) { ?>
    <?=$output?> = <?=$variables[$counter % 5]?>(<?=floor($counter / 5)?>);
<?  } ?>
    <?=$count?> = count;
  }
};

<?
    return [
        'kind'           => 'GLA',
        'name'           => $className,
        'system_headers' => $sys_headers,
        'user_headers'   => $user_headers,
        'lib_headers'    => $lib_headers,
        'input'          => $inputs,
        'output'         => $outputs,
        'result_type'    => 'single',
    ];
}
?>