import 'package:flutter_test/flutter_test.dart';
import 'package:allarme_camper/main.dart';

void main() {
  testWidgets('App visualizza titolo e bottone Connetti', (WidgetTester tester) async {
    await tester.pumpWidget(const MyApp());
    expect(find.text('Allarme Camper'), findsOneWidget);
    expect(find.text('Connetti'), findsWidgets);
  });
}